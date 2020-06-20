

void Always_loop()
{
  OTA_Check();
}


void Fast_Loop()
{ 
  Handle_ButtonPress();
  server.handleClient();  
  
  Handle_ButtonPress();
  MQTT_loop();  
  
  Handle_ButtonPress();
  Check_MotionState();
  
  Handle_ButtonPress();
  Handle_Timer();
}


void Slow_Loop()
{  
  MQTT_publish();  

  if(MQTT_PIR_last_ON_msg_state == false && TMR_Status == 0)
  {
  	int a = 0000;
    Send_data_SPI(5, a);			// off
  }
}



void Mid1_Loop()
{  
  //Handle_Timer();
}


void Mid2_Loop()
{
  
}



void VerySlow_Loop()
{
  
}

