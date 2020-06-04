

/******************** LIB **************************************/
#define MQTT_MAX_PACKET_SIZE 1024  // < ----- Change in lib: This is because the defaul maxium length is 128b. So just go to PubSubClient.h and change #define MQTT_MAX_PACKET_SIZE 128 to #define MQTT_MAX_PACKET_SIZE 1024
#include <PubSubClient.h>
#include <ArduinoJson.h>


/********************* Var *************************************/
WiFiClient espClient;
PubSubClient client(espClient);

long lastReconnectAttempt = 0;

/******************** Config **************************************/
const char* mqtt_server     = SECRET_MQTT_Server;
const char* mqtt_user       = SECRET_MQTT_User;
const char* mqtt_password   = SECRET_MQTT_Pass;

// Motion Sensor
#define MQTT_CONFIG_PIR        "homeassistant/sensor/PIR/Kitchen/config"
#define MQTT_TOPIC_STATE_PIR   "homeassistant/sensor/PIR/Kitchen/state"

unsigned long MQTT_PIR_heartbeat_timestamp;
unsigned long MQTT_PIR_last_ON_msg_timestamp;
bool MQTT_PIR_last_ON_msg_state;


/**************** External Functions ************************************/

void MQTT_Config()
{
	client.setServer(mqtt_server, SECRET_MQTT_Port);
 	client.setCallback(MQTT_MessageRecd_callback);
}


void MQTT_loop()
{
	if (!client.connected())
    	MQTT_reconnect();              
  
  	client.loop();
}


void MQTT_publish()
{   
    MQTT_PIR_heartbeat();
}


/**************** Internal Functions ************************************/

void MQTT_reconnect() 
{
  if (millis()/1000 - lastReconnectAttempt > 30 || lastReconnectAttempt == 0) 
  {
      Serial.println("MQTT reconnecting");
      if (client.connect(DeviceHostName, mqtt_user, mqtt_password)) 
      {
        //MQTT_publish_config();  
        Serial.println("MQTT connected");
      }
      lastReconnectAttempt = millis()/1000;
  }
}


void MQTT_MessageRecd_callback(char* topic, byte* payload, unsigned int length) 
{
 
}


//-------------------- PIR MQTT -----------------------------


// Send Motion sensed msg
void MQTT_publish_PIR(bool PIR_State)
{   
  if(millis()/1000 - MQTT_PIR_last_ON_msg_timestamp > 30)  // Atleast 30 sec have passsed since last positive "ON" transmission
  {
    MQTT_PIR_last_ON_msg_state = PIR_State;
    MQTT_PIR_last_ON_msg_timestamp = millis()/1000;
    MQTT_PIR_heartbeat_timestamp = MQTT_PIR_last_ON_msg_timestamp;  

    if(PIR_State)
      client.publish(MQTT_TOPIC_STATE_PIR, "ON", true);
    else
      client.publish(MQTT_TOPIC_STATE_PIR, "OFF", true);
  }
  //Serial.println(data);
}


// Acts as heartbeat and also sends zero state (no motion) 
void MQTT_PIR_heartbeat()
{
  if(millis()/1000 - MQTT_PIR_heartbeat_timestamp > 60 || MQTT_PIR_heartbeat_timestamp==0)  // send every 60 sec
  {
    MQTT_PIR_heartbeat_timestamp = millis()/1000;
    client.publish(MQTT_TOPIC_STATE_PIR, "OFF", true);
  }
}