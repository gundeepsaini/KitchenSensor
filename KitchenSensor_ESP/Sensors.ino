


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
      int a = 8888;
      Send_data_SPI(11, a);       // send msg "On"
    }
}



void Handle_ButtonPress()
{ 
  if(sp_mins == 0)
    {
      buttonPress_timestamp = millis()/1000;
      TMR_start_time = buttonPress_timestamp;
    }
  
  if(millis()/1000 - buttonPress_timestamp > 20 )   // time before which tmr change is allowed
    {
      sp_mins = 0;
      Send_data_SPI(5, 0, 0);
      delay(2000);
    }
  else
  {
    switch(sp_mins)
    {
      case 0:
          sp_mins = 5;
          break;

      case 5:
          sp_mins = 10;
          break;

      case 10:
          sp_mins = 20;
          break;

      case 20:
          sp_mins = 30;
          break;
    }
    Send_data_SPI(31, int(sp_mins), 0);  
  }
  
  //Serial.println(sp_mins);
}



void Handle_Timer()
{
  if(sp_mins > 0)
  { 
    long TMR_elapsed = millis()/1000 - TMR_start_time;
    int sp_secs = sp_mins * 60;

    if(TMR_elapsed < 10)              // display the setpoint for x secs
      {
        //Send_data_SPI(31, int(sp_mins), 0);
      }
    else      
      {
        if(sp_secs - TMR_elapsed > 0)   // set point not reached
        {
          TMR_mins_left = ((sp_secs) - TMR_elapsed) / 60;    
          TMR_secs_left = ((sp_secs) - TMR_elapsed) - (TMR_mins_left*60);
          
          //Serial.print(TMR_elapsed);
          //Serial.print("    -  ");
          //Serial.print(TMR_mins_left);
          //Serial.print(":");
          //Serial.println(TMR_secs_left);

          Send_data_SPI(31, TMR_mins_left, TMR_secs_left);
        }
        else
        {
          // set point exceeded

          // send MQTT msg
          // blink the time
          
          TMR_mins_left = (TMR_elapsed - sp_secs) / 60;    
          TMR_secs_left = (TMR_elapsed - sp_secs) - (TMR_mins_left*60);      

          //Serial.print(TMR_elapsed);
          //Serial.print("    -  ");
          //Serial.print(TMR_mins_left);
          //Serial.print(":");
          //Serial.println(TMR_secs_left);
          
          if(millis() - blink_tmr > 1000)
            {
              Send_data_SPI(32, TMR_mins_left, TMR_secs_left);
              blink_tmr = millis();
            }
          else
            Send_data_SPI(5, 0, 0);
            
          if(Motion_State)
            {
              sp_mins = 0;
              Send_data_SPI(5, 0, 0);
            }            
        }
      }
  }
}