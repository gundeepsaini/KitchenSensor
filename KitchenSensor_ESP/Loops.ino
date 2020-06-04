

void Always_loop()
{
  OTA_Check();
}


void Fast_Loop()
{   
  server.handleClient();
  MQTT_loop();
  Check_ButtonState();
  Check_MotionState();
}



void Mid1_Loop()
{  

}


void Mid2_Loop()
{
  
}



void Slow_Loop()
{  
  MQTT_publish();  
}


void VerySlow_Loop()
{
  
}

