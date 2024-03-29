

// https://pubsubclient.knolleary.net/api


/******************** LIB **************************************/
#define MQTT_MAX_PACKET_SIZE 1024  // < ----- Change in lib: This is because the defaul maxium length is 128b. So just go to PubSubClient.h and change #define MQTT_MAX_PACKET_SIZE 128 to #define MQTT_MAX_PACKET_SIZE 1024
#include "src/pubsubclient/PubSubClient.h"
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
#define MQTT_CONFIG_PIR        "HA/KitchenTMR/PIR/config" 
#define MQTT_TOPIC_STATE_PIR   "HA/KitchenTMR/PIR/state"  

// Kitchen Timer
#define MQTT_CONFIG_TMR        "HA/KitchenTMR/TMR/config" 
#define MQTT_TOPIC_STATE_TMR   "HA/KitchenTMR/TMR/state"  

// Will Topic - Availability
#define MQTT_TOPIC_WILL       "HA/KitchenTMR/status"     
#define MQTT_OFFLINE          "Offline"
#define MQTT_ONLINE           "Active"

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

    MQTT_publish_TMR_elapsed(TMR_Status, TMR_secs_left);
}


/**************** Internal Functions ************************************/


void MQTT_reconnect() 
{
  if (millis()/1000 - lastReconnectAttempt > 30 || lastReconnectAttempt == 0) 
  {
      Serial.println("MQTT reconnecting");
      
      //boolean connect (clientID, [username, password], [willTopic, willQoS, willRetain, willMessage], [cleanSession])
      if (client.connect(DeviceHostName, mqtt_user, mqtt_password, MQTT_TOPIC_WILL, 1, true, MQTT_OFFLINE)) 
      { 
        //MQTT_publish_config();  
        Serial.println("MQTT connected");
        client.publish(MQTT_TOPIC_WILL, MQTT_ONLINE, true);
      }
      lastReconnectAttempt = millis()/1000;
  }

  if(First_Bootup == 1)
    {
      First_Bootup = 0;
      MQTT_publish_TMR_elapsed(0, 0);
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
    if(PIR_State)
      client.publish(MQTT_TOPIC_STATE_PIR, "ON", true);
    else
      client.publish(MQTT_TOPIC_STATE_PIR, "OFF", true);

    MQTT_PIR_last_ON_msg_state = PIR_State;
    MQTT_PIR_last_ON_msg_timestamp = millis()/1000;
    MQTT_PIR_heartbeat_timestamp = MQTT_PIR_last_ON_msg_timestamp;  

    if(TMR_Status == 0)
      Send_data_SPI(11, 1);       // send msg "On"
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
    MQTT_PIR_last_ON_msg_state = false;
  }
}



//-------------------- TIMER MQTT -----------------------------

int TMR_state_MQTT_old;

// Send msg
void MQTT_publish_TMR_elapsed(int TMR_state_MQTT, int TMR_secs_left_MQTT)
{   
  if(millis()/1000 - MQTT_TMR_last_msg_timestamp > 10 || (TMR_state_MQTT != TMR_state_MQTT_old))  // Atleast x sec have passsed since last transmission
  { 
      TMR_state_MQTT_old = TMR_state_MQTT;

    // Use arduinojson.org/v6/assistant to compute the capacity.
      const size_t capacity = JSON_OBJECT_SIZE(10);
      DynamicJsonDocument doc(capacity);

      doc["Status"]       = String(TMR_state_MQTT);
      doc["TimeLeft"]     = String(TMR_secs_left_MQTT);
      
      char data[256];
      serializeJson(doc, data, sizeof(data));
      client.publish(MQTT_TOPIC_STATE_TMR, data, true);
      Serial.println(data);

      MQTT_TMR_last_msg_timestamp = millis()/1000;
  }
}

