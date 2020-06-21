
int segPins[]       = {18, 17, 15, 1, 0, 14, 19, 16 };    //  a  b  c  d  e  f  g  .
int displayPins[]   = {5, 6, 9, 4, 3};                    //  D1...D5   LSD....MSD
int DelayIntensity  = 5;


// 7 segment code table
byte segCode[16][8] = 
{
  //  a  b  c  d  e  f  g  .
  { 0, 0, 0, 0, 0, 0, 1, 1},  // 0
  { 1, 0, 0, 1, 1, 1, 1, 1},  // 1
  { 0, 0, 1, 0, 0, 1, 0, 1},  // 2
  { 0, 0, 0, 0, 1, 1, 0, 1},  // 3
  { 1, 0, 0, 1, 1, 0, 0, 1},  // 4
  { 0, 1, 0, 0, 1, 0, 0, 1},  // 5
  { 0, 1, 0, 0, 0, 0, 0, 1},  // 6
  { 0, 0, 0, 1, 1, 1, 1, 1},  // 7
  { 0, 0, 0, 0, 0, 0, 0, 1},  // 8
  { 0, 0, 0, 0, 1, 0, 0, 1},  // 9
  
  { 1, 1, 1, 1, 1, 1, 1, 1},
  { 1, 1, 1, 1, 1, 1, 1, 1},  // 11 - OFF
  { 1, 1, 1, 1, 1, 1, 0, 1},  // 12 - dash
  { 0, 1, 1, 0, 0, 0, 1, 1},  // 13 - C
  { 1, 0, 0, 1, 0, 0, 0, 1},  // 14 - H 
  { 1, 1, 1, 1, 1, 1, 1, 0}   // 15 - dot
};


void Config_Hardware()
{
  for (int i=0; i < 8; i++)
    {
      pinMode(segPins[i], OUTPUT);
      digitalWrite(segPins[i], HIGH);
    }

  for (int i=0; i < 5; i++)
   {
    pinMode(displayPins[i], OUTPUT);
    digitalWrite(segPins[i], LOW);
   }
}


void Display_Number_int(int number)
{
  // displays integer numbers between -9,999 to +99,999

  bool negative = 0;
  
  if(number < 0)
    {
      number = number * -1;
      negative = 1;
    }  

  int n1 =  number % 10;
  int n2 = (number % 100)/10;            
  int n3 = (number % 1000)/100;        
  int n4 = (number % 10000)/1000;        
  int n5 = (number % 100000)/10000; 

  if(!negative)
    if(number < 10)
      refreshDisplay(11,11,11,11,n1);
    else 
      if(number < 100)
        refreshDisplay(11,11,11,n2,n1);  
      else 
        if(number < 1000)
          refreshDisplay(11,11,n3,n2,n1);
        else 
          if(number < 10000)
            refreshDisplay(11,n4,n3,n2,n1);
          else
            if(number <= 99999) 
              refreshDisplay(n5,n4,n3,n2,n1);
            else
              refreshDisplay(n5,n4,n3,n2,n1,5);
  else
    if(number < 10)
      refreshDisplay(11,11,11,12,n1);
    else 
      if(number < 100)
        refreshDisplay(11,11,12,n2,n1);  
      else 
        if(number < 1000)
          refreshDisplay(11,12,n3,n2,n1);
        else 
          if(number < 10000)
            refreshDisplay(12,n4,n3,n2,n1);
          else
            refreshDisplay(12,n4,n3,n2,n1,5);

}




void Display_Temperature(float number)
{
  // displays temperature (float) in format xx.xC between -99.9C to +999.9C

  bool negative = 0;  
  if(number < 0)
    {
      number = number * -1;
      negative = 1;
    }

  int d1 = int(number * 10) % 10;
  int n1 = (int(number) % 10);
  int n2 = (int(number) % 100)/10;
  int n3 = (int(number) % 1000)/100;  
  
  if(!negative)
    if(number < 10.0)
      refreshDisplay(11,11,n1,d1,13,3);
    else 
      if(number < 100.0)
        refreshDisplay(11,n2,n1,d1,13,3);
      else 
          refreshDisplay(n3,n2,n1,d1,13,3);
  else
    if(number < 10.0)
      refreshDisplay(11,12,n1,d1,13,3);
    else 
      refreshDisplay(12,n2,n1,d1,13,3);
}


unsigned long last_time_dash;
int last_time_dash_state=11;

void Display_Time(int t_hr, int t_min)
{
  // displays time in format hh-mm between 00-00 to 23-59 (time display) or -9-59 to 99:59 (time counter)

  bool negative = 0;  
  if(t_hr < 0)
    {
      t_hr = t_hr * -1;
      negative = 1;
    }

  int h1 = ((t_hr) % 10);
  int h2 = ((t_hr) % 100)/10;
  int m1 = ((t_min) % 10);
  int m2 = ((t_min) % 100)/10;

  // handles blinking dash for time
  if(millis()/100 - last_time_dash > 5)
  {
    if(last_time_dash_state == 12)
      last_time_dash_state = 11;
    else
      last_time_dash_state = 12;
    
    last_time_dash = millis()/100;
  }

  if(!negative)
    if(t_hr < 10)
      refreshDisplay(11,h1,last_time_dash_state,m2,m1);
    else
      if(t_hr < 100) 
        refreshDisplay(h2,h1,last_time_dash_state,m2,m1);
      else
        refreshDisplay(h2,h1,last_time_dash_state,m2,m1,5);
  else
    if(t_hr < 10)    
      refreshDisplay(12,h1,last_time_dash_state,m2,m1);
    else
      refreshDisplay(12,h1,last_time_dash_state,m2,m1,5);
}



void refreshDisplay(int digit5, int digit4, int digit3, int digit2, int digit1)
{   
  
  digitalWrite(displayPins[0],HIGH); 
  digitalWrite(displayPins[1],LOW );
  digitalWrite(displayPins[2],LOW );
  digitalWrite(displayPins[3],LOW );
  digitalWrite(displayPins[4],LOW );
  setSegments(digit1);
  delay(DelayIntensity);

  digitalWrite(displayPins[0],LOW); 
  if(digit2 != 11)
    digitalWrite(displayPins[1],HIGH );
  digitalWrite(displayPins[2],LOW );
  digitalWrite(displayPins[3],LOW );
  digitalWrite(displayPins[4],LOW );
  setSegments(digit2);
  delay(DelayIntensity);

  digitalWrite(displayPins[0],LOW); 
  digitalWrite(displayPins[1],LOW );
  if(digit3 != 11)
    digitalWrite(displayPins[2],HIGH );
  digitalWrite(displayPins[3],LOW );
  digitalWrite(displayPins[4],LOW );
  setSegments(digit3);
  delay(DelayIntensity);

  digitalWrite(displayPins[0],LOW); 
  digitalWrite(displayPins[1],LOW );
  digitalWrite(displayPins[2],LOW );
  if(digit4 != 11)
    digitalWrite(displayPins[3],HIGH );
  digitalWrite(displayPins[4],LOW );
  setSegments(digit4);
  delay(DelayIntensity);

  digitalWrite(displayPins[0],LOW); 
  digitalWrite(displayPins[1],LOW );
  digitalWrite(displayPins[2],LOW );
  digitalWrite(displayPins[3],LOW );
  if(digit5 != 11)
    digitalWrite(displayPins[4],HIGH );
  setSegments(digit5);
  delay(DelayIntensity);

  digitalWrite(displayPins[0],LOW); 
  digitalWrite(displayPins[1],LOW );
  digitalWrite(displayPins[2],LOW );
  digitalWrite(displayPins[3],LOW );
  digitalWrite(displayPins[4],LOW );
}


void setSegments(int n)
{  
    for (int i=0; i < 8; i++)
      digitalWrite(segPins[i], segCode[n][i]);
}


void setSegments_with_decimal(int n)
{  
  byte a[8];
  for(int i=0;i<8;i++)              
  {
    a[i] = segCode[n][i] && segCode[15][i];
    digitalWrite(segPins[i], a[i]);
  }     
}


void refreshDisplay(int digit5, int digit4, int digit3, int digit2, int digit1, int decimal_on_digit)
{   
  
  digitalWrite(displayPins[0],HIGH); 
  digitalWrite(displayPins[1],LOW );
  digitalWrite(displayPins[2],LOW );
  digitalWrite(displayPins[3],LOW );
  digitalWrite(displayPins[4],LOW );
  if(decimal_on_digit == 1)
    setSegments_with_decimal(digit1);
  else
    setSegments(digit1);
  delay(DelayIntensity);

  digitalWrite(displayPins[0],LOW); 
  if(digit2 != 11)
    digitalWrite(displayPins[1],HIGH );
  digitalWrite(displayPins[2],LOW );
  digitalWrite(displayPins[3],LOW );
  digitalWrite(displayPins[4],LOW );
  if(decimal_on_digit == 2)
    setSegments_with_decimal(digit2);
  else
    setSegments(digit2);
  delay(DelayIntensity);

  digitalWrite(displayPins[0],LOW); 
  digitalWrite(displayPins[1],LOW );
  if(digit3 != 11)
    digitalWrite(displayPins[2],HIGH );
  digitalWrite(displayPins[3],LOW );
  digitalWrite(displayPins[4],LOW );
  if(decimal_on_digit == 3)
    setSegments_with_decimal(digit3);
  else
    setSegments(digit3);
  delay(DelayIntensity);

  digitalWrite(displayPins[0],LOW); 
  digitalWrite(displayPins[1],LOW );
  digitalWrite(displayPins[2],LOW );
  if(digit4 != 11)
    digitalWrite(displayPins[3],HIGH );
  digitalWrite(displayPins[4],LOW );
  if(decimal_on_digit == 4)
    setSegments_with_decimal(digit4);
  else
    setSegments(digit4);
  delay(DelayIntensity);

  digitalWrite(displayPins[0],LOW); 
  digitalWrite(displayPins[1],LOW );
  digitalWrite(displayPins[2],LOW );
  digitalWrite(displayPins[3],LOW );
  if(digit5 != 11)
    digitalWrite(displayPins[4],HIGH );
  if(decimal_on_digit == 5)
    setSegments_with_decimal(digit5);
  else
    setSegments(digit5);
  delay(DelayIntensity);

  digitalWrite(displayPins[0],LOW); 
  digitalWrite(displayPins[1],LOW );
  digitalWrite(displayPins[2],LOW );
  digitalWrite(displayPins[3],LOW );
  digitalWrite(displayPins[4],LOW );
}


void decimal()
{

 // handle decimal by doing bitwise AND of a) number command and b) decimal command

  byte a[8];
  int n = 0;

  for(int k = 0; k<10;k++)
    {
      Serial.print(k);
      Serial.print(" :");
      for(int i=0;i<8;i++)       
     {          
       a[i] = segCode[k][i] && segCode[15][i]; 
       Serial.print(a[i]);         
     }
     Serial.println();
   }
}




//******************* TEST FUNCTIONS **********************************************
/*

  long startTime = 0;  
  int looptime = 100;
  long i = 0, j = 0, k=0;
  

void test_Display_Number_int()
{

  Display_Number_int(i);
  
  if ((millis() - startTime) > looptime)
   {
      j++;
      //i++;

      if(j % 2 == 0)
        i = j*-1;
      else
        i = j*1;
      
      startTime = millis();
   }
}


void test_Display_Temperature()
{

  float k = float(i)/10.0;
  Display_Temperature(k);
  
  if ((millis() - startTime) > looptime)
   {
      j++;
      //i++;
      //k=k+0.1;

      if(j % 2 == 0)
        i = j*-1;
      else
        i = j*1;
      
      startTime = millis();
   }
}


void test_Display_Time()
{

  Display_Time(i,j);
  
  if ((millis() - startTime) > looptime)
   {
      j++;
      //i++;

      if(j > 59)
      {        
        k ++;
        j = 0;
        if(k % 2 == 0)
          i = k*-1;
        else
          i = k* 1;
      }    
      startTime = millis();
   }
}

*/
