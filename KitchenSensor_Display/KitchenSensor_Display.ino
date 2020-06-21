/*-----------------------------------------

            Display Module

Date: June 2020 (v01)
Device: Atmega8 (Slave), 5x 7 segment display 

Description:
  - 7 segment display on my custom designed PCB (from wireless LCD project)
  - Circuit description details available in OneNote notebook
  - Atmega8 programmed using ArduinoISP 
  - Bootloader from MCUdude: https://github.com/MCUdude/MiniCore

  
To-Do:
  - 

------------------------------------------- */

#include <SPI.h>

byte buffer_comm[4]; 
byte buffer_index=0;


int Mode = 0;
int data1 = 0;
int data2 = 0;
int data3 = 0;
int number = 0;


void setup() 
{
	// Initial delay for master to get configured on SPI lines
  delay(5000);
  
  Config_Hardware();
  SPI_Config();
}



void loop()
{

  SPI_ProcessData_Received();

  switch(Mode)
  {
    case 0: 
      Display_Number_int(0);
      break;

    case 11: // Integer - positive      
      Display_Number_int(number);
      break;

    case 12: // Integer - negative
      Display_Number_int(number * -1);
      break;

    case 21: // Temp - positive
      Display_Temperature(number/10.0);
      break;

    case 22: // Temp - negative
      Display_Temperature((number * -1) / 10.0);
      break;     

    case 31: // Time - positive      
      Display_Time(data1, data2);
      break; 

    case 32: // Time - negative      
      Display_Time(data1 * -1, data2);
      break;      
  }
	
}
