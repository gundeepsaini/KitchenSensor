


void Hardware_Config()
{
  //pinMode(Button_Pin, INPUT);
  pinMode(PIR_Pin, INPUT);
  
  int presses = 5;        // Number of presses
  int timeout = 2000;     // Timeout
  int duration = 2000;    // Duration

  button.begin();  
  button.onPressed(Handle_ButtonPress);
  //button.onSequence(presses, timeout, onSequenceMatched);
}



void Check_MotionState()
{
  Motion_State = digitalRead(PIR_Pin);

  if(Motion_State)
    {
      MQTT_publish_PIR(Motion_State);
      int a = 11111;
      Send_data_SPI(11, a);       // send msg "On"
    }
}



void Handle_ButtonPress()
{
   Serial.println("button pressed");
   sp_mins = 5;
   TMR_start_time = millis()/1000; 
}



void Handle_Timer()
{
  if(sp_mins > 0)
  { 
    long TMR_elapsed = millis()/1000 - TMR_start_time;
    //long TMR_left    = (sp_mins*60) - TMR_elapsed;

    TMR_mins_left = ((sp_mins*60) - TMR_elapsed) % 60;
    TMR_secs_left = TMR_elapsed - (TMR_mins_left*60);

    Serial.print(TMR_mins_left);
    Serial.print(":");
    Serial.println(TMR_secs_left);

    Send_data_SPI(32, TMR_mins_left, TMR_secs_left);

  }
}