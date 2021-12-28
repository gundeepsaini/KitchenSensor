

void Always_loop()
{
  OTA_Check();
}


void Fast_Loop()
{ 
  if(Button_Pressed)
    Handle_ButtonPress_Actions();

  server.handleClient();  
  MQTT_loop();  
  Check_MotionState();
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
  Time_NTP_Update();
}

