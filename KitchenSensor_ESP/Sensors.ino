


void Hardware_Config()
{
  pinMode(Button_Pin, INPUT);
  pinMode(PIR_Pin, INPUT);
  
  int presses = 5;        // Number of presses
  int timeout = 2000;     // Timeout
  int duration = 2000;    // Duration

  //button.begin();  
  //button.onPressed(Handle_ButtonPress);
  //button.onSequence(presses, timeout, onSequenceMatched);
}



void Check_MotionState()
{
  Motion_State = digitalRead(PIR_Pin);

  if(Motion_State)
    {
      MQTT_publish_PIR(Motion_State);
      //int a = 8888;
      //Send_data_SPI(11, a);       // send msg "On"

      if(TMR_Status == 2)
      {
        TMR_Status = 3;
        time_since_tmr_ack = millis()/1000;
      }
    }
}




void Handle_ButtonPress()
{ 
  bool ButtonState = digitalRead(Button_Pin);

  if(ButtonState)
  {
    if(millis() - time_since_last_buttonpress > 500)     // Min time after a button press for debounc handling
    {
      time_since_last_buttonpress = millis();

      switch(TMR_Status)
      {
        case 0:
            TMR_Status = 1;
            sp_mins = 5;
            TMR_start_time = time_since_last_buttonpress/1000;            
            break;

        case 1:
            if(time_since_last_buttonpress/1000 - TMR_start_time < 15)    // Min time to change set point
            {
              switch(sp_mins)
                {
                  case 5:
                      sp_mins = 10;
                      break;

                  case 10:
                      sp_mins = 20;
                      break;

                  case 20:
                      sp_mins = 30;
                      break;

                  case 30:
                      sp_mins = 0;
                      TMR_start_time = 0;
                      time_since_last_buttonpress = time_since_last_buttonpress + 2000; // Additional delay to prevent retrigger
                      TMR_Status = 0;
                      TMR_secs_left = 0;
                      Send_data_SPI(5,0,0); // OFF
                      MQTT_publish_TMR_elapsed(TMR_Status, TMR_secs_left);
                      delay(2000);
                      break;
                }
            }
            else
            {
              sp_mins = 0;
              TMR_start_time = 0;
              time_since_last_buttonpress = time_since_last_buttonpress + 2000; // Additional delay to prevent retrigger
              TMR_Status = 0;
              TMR_secs_left = 0;
              Send_data_SPI(5,0,0); // OFF
              delay(1000);
              MQTT_publish_TMR_elapsed(TMR_Status, TMR_secs_left);
              delay(1000);
            }
            break;
        
        case 2:
            TMR_Status = 3;
            time_since_tmr_ack = time_since_last_buttonpress/1000;
            break;             
      }

      if(sp_mins > 0)
        Send_data_SPI(31,sp_mins,0);
    }
  }
}


/*

TMR_Status:
0 - OFF
1 - Running  (down counting)
2 - Complete (up   counting)
3 - Acknowledged (stopped)

*/



void Handle_Timer()
{
  if(TMR_Status > 0)
  { 
    long TMR_time_elapsed = millis()/1000 - TMR_start_time;
    int sp_secs = sp_mins * 60;

    switch(TMR_Status)
    {
      case 1: // Timer running (down counting)
          
          if(TMR_time_elapsed >= sp_secs)
            {
              TMR_Status = 2;
              break;
            }
          else
          {
            TMR_secs_left = sp_secs - TMR_time_elapsed;
            TMR_mins_left_display = ((sp_secs) - TMR_time_elapsed) / 60;    
            TMR_secs_left_display = ((sp_secs) - TMR_time_elapsed) - (TMR_mins_left_display*60);
            Send_data_SPI(31, TMR_mins_left_display, TMR_secs_left_display);
          }
          break;

      case 2: // Timer complete (up   counting)
            TMR_secs_left = TMR_time_elapsed - sp_secs;
            TMR_mins_left_display = (TMR_time_elapsed - sp_secs) / 60;    
            TMR_secs_left_display = (TMR_time_elapsed - sp_secs) - (TMR_mins_left_display*60); 
            Send_data_SPI(32, TMR_mins_left_display, TMR_secs_left_display);
          break;

      case 3: // Timer Acknowledged (stopped)
          if(millis()/1000 - time_since_tmr_ack > 15)
          {
            TMR_Status = 0;
            time_since_tmr_ack = 0;
            TMR_secs_left = 0;
            sp_mins = 0;
            TMR_start_time = 0;
            Send_data_SPI(5, 0, 0);                
          }
          break;          
    }

    MQTT_publish_TMR_elapsed(TMR_Status, TMR_secs_left);
  }
}


/*


  if(sp_mins > 0)
  { 
    

    if(TMR_elapsed < 10)              // display the setpoint for x secs
      {
        //Send_data_SPI(31, int(sp_mins), 0);
      }
    else      
      {
        if(sp_secs - TMR_elapsed > 0)   // set point not reached
        {
          TMR_complete = 0;
          TMR_mins_left = ((sp_secs) - TMR_elapsed) / 60;    
          TMR_secs_left = ((sp_secs) - TMR_elapsed) - (TMR_mins_left*60);

          Send_data_SPI(31, TMR_mins_left, TMR_secs_left);
        }
        else
        {
          // set point exceeded
          TMR_complete = 1;
          TMR_mins_left = (TMR_elapsed - sp_secs) / 60;    
          TMR_secs_left = (TMR_elapsed - sp_secs) - (TMR_mins_left*60);      
          
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
              TMR_mins_left = 0;
              TMR_secs_left = 0;
              TMR_complete = 0;
              Send_data_SPI(5, 0, 0);
              MQTT_publish_TMR_elapsed(false);  
            }
          else
            // send MQTT msg
            MQTT_publish_TMR_elapsed(true);            
        }
      }
  }



void Handle_ButtonPress_old()
{ 
  if(sp_mins == 0)
    {
      // First time button pressed
      buttonPress_timestamp = millis()/1000;
      TMR_start_time = buttonPress_timestamp;
    }
  
  if(millis()/1000 - buttonPress_timestamp > 20 )   
    {
      // time after which tmr change is not allowed. It is switched off.
      sp_mins = 0;
      TMR_mins_left = 0;
      TMR_secs_left = 0;
      TMR_complete = 0;
      Send_data_SPI(5, 0, 0);
      MQTT_publish_TMR_elapsed(false); 
      delay(1000);
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
    delay(1000);  
  }
  
  //Serial.println(sp_mins);
}
*/