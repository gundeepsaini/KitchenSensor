 /*-----------------------------------------

            Kitchen Sensor Module

Date: June 2020 (v01)
Device: ESP8266 NodeMcU (Master), Atmega8 (Slave)

Description:
  - Motion Sensor unit for triggering Kitchen lights
  - Additionally connected to Atmega8 over SPI to display
    information on 5 x 7 segment display
  - Secondary use is as a digital kitchen timer unit with the button
  - Time left is displayed on 7 segment when it is activated


Note:
  - Reset ESP after upload, disable IPv6, 
      reset adapter, allow port in firewall
  - send "OTA" on blynk terminal to enter dedicated mode
    or navigate to "ip/OTA" for OTA through web portal
  
To-Do:
  - 

Changes:
  - vXX | XX.XX.2020
    Changes 


------------------------------------------- */


/* ------------- LIB ------------------------------ */
#include "Secrets.h"
#include <ArduinoOTA.h>
#include <ESP8266WebServer.h>
#include <SPI.h>
#include <EasyButton.h>

// remove the unnecessary libs
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiClient.h>
#include <WiFiUdp.h>


/* ------------- CONFIG VAR ------------------------------ */
unsigned long looptime_Fast = 0;    // in secs
unsigned long looptime_Mid1 = 1;    // in secs
unsigned long looptime_Mid2 = 10;   // in secs
unsigned long looptime_Slow       = 1 * (60);      // in mins
unsigned long looptime_VerySlow   = 5 * (60);      // in mins

#define Button_Pin  4
#define PIR_Pin     5


/* ------------- VAR ------------------------------ */
const char* ssid             = SECRET_WIFI_SSID2;
const char* pass             = SECRET_WIFI_PASS2;
const char* DeviceHostName   = SECRET_Device_Name6;
const char* OTA_Password     = SECRET_Device_OTA_PASS; 
unsigned long lastrun_fast, lastrun_Mid1;
unsigned long lastrun_Mid2, lastrun_slow, lastrun_Veryslow;
unsigned long lastrun_30mins,lastrun_OTA;
bool OTA_Mode=0;
String webpage1="";
bool Motion_State = 0, Button_State = 0;
int sp_mins;
byte TMR_mins_left_display, TMR_secs_left_display; 
unsigned long TMR_start_time;
unsigned long MQTT_PIR_heartbeat_timestamp;
unsigned long MQTT_PIR_last_ON_msg_timestamp, MQTT_TMR_last_msg_timestamp = -10;
bool MQTT_PIR_last_ON_msg_state;
unsigned long buttonPress_timestamp=0;
unsigned long blink_tmr = 0;
bool TMR_complete = 0;
int TMR_Status=0, TMR_secs_left=0;
unsigned long time_since_last_buttonpress=0, time_since_tmr_ack=0;
bool First_Bootup = 1;

ESP8266WebServer server(80);
EasyButton button(Button_Pin);


void setup() 
{
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  
  Serial.begin(115200);
  Serial.println(DeviceHostName);

  wifi_station_set_hostname(DeviceHostName);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) 
  {
    Serial.println("Wifi Error! Rebooting in 30s...");
    delay(30 *1000);
    digitalWrite(LED_BUILTIN, HIGH);
    Restart_ESP();
  }
  
  Serial.print("Connected to ");
  Serial.print(WiFi.SSID());
  Serial.print(", IP Address: ");
  Serial.println(WiFi.localIP());
  
  Comm_SPI_Config();
  Time_NTP_Config();  
  OTA_Config();
  WebServer_Config();
  MQTT_Config();
  Hardware_Config();
  
  digitalWrite(LED_BUILTIN, HIGH);
  Serial.println("Ready");
}


void loop() 
{
  // Always
  Always_loop();

  // Fast Loop
 if(!OTA_Mode && ((millis()/1000 - lastrun_fast > looptime_Fast) || lastrun_fast ==0))
  {
    lastrun_fast = millis()/1000;
    Fast_Loop();
  }

/*
  // Mid1 Loop
 if(!OTA_Mode && ((millis()/1000 - lastrun_Mid1 > looptime_Mid1) || lastrun_Mid1 ==0))
  {
    lastrun_Mid1 = millis()/1000;
    Mid1_Loop();
  }


  // Mid2 Loop
 if(!OTA_Mode && ((millis()/1000 - lastrun_Mid2 > looptime_Mid2) || lastrun_Mid2 ==0))
  {
    lastrun_Mid2 = millis()/1000;
    Mid2_Loop();
  }
*/
  // Slow Loop
 if(!OTA_Mode && ((millis()/1000 - lastrun_slow > looptime_Slow) || lastrun_slow ==0))
  {
    lastrun_slow = millis()/1000;
    Slow_Loop();
  }

/*

    // Very Slow Loop
 if(!OTA_Mode && ((millis()/1000 - lastrun_Veryslow > looptime_VerySlow) || lastrun_Veryslow ==0))
  {
    lastrun_Veryslow = millis()/1000;
    VerySlow_Loop();
  }

*/
}



void Restart_ESP()
{
  Serial.println("Restarting ESP");
  ESP.restart();
  delay(1000);
  while(1);
}
