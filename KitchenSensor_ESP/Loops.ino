

void Always_loop()
{
  OTA_Check();
}


void Fast_Loop()
{ 
  button.read();
  server.handleClient();  
  button.read();
  MQTT_loop();  
  button.read();
  Check_MotionState();
  button.read();
  Handle_Timer();
}



void Mid1_Loop()
{  
  //Handle_Timer();
}


void Mid2_Loop()
{
  
}



void Slow_Loop()
{  
  MQTT_publish();  

  if(MQTT_PIR_last_ON_msg_state == false)
  {
  	int a = 0000;
    //Send_data_SPI(1, a);			// off
  }
}


void VerySlow_Loop()
{
  
}

