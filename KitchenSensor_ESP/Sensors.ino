


void Hardware_Config()
{
  pinMode(Button_Pin, INPUT);
  pinMode(PIR_Pin, INPUT);
}



void Check_ButtonState()
{
  if(digitalRead(Button_Pin))
  {
    //Serial.println("ButtonPress!");
  }
}


void Check_MotionState()
{
  if(digitalRead(PIR_Pin))
    {
      Motion_State = true;
      MQTT_publish_PIR(Motion_State);
      int a = 11111;
      Send_data_SPI(11, a);
    }
  else
    {
      Motion_State = false;
      Send_data_SPI(1, 0);
    }
}