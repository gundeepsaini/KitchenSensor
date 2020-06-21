/*


Communication definition:

Byte 1: Mode Information 
Byte 2: data 1 byte MSB
Byte 3: data 2 byte LSB
Byte 4: for future use 


Modes:
  0   - display 0
  11  - display positive integer
  12  - display negative integer
  21  - display positive temperature (number is treated as 123.4 sent as 1234)
  22  - display negative temperature (number is treated as 123.4 sent as 1234)
  31  - display positive time (data 1 is hour and data 2 is mins)
  32  - display negative time (data 1 is hour and data 2 is mins)


*/




void SPI_Config()
{  
  pinMode(MISO, OUTPUT);
  SPCR |= _BV(SPE); 
  SPI.attachInterrupt(); 
  buffer_index=0;
}


bool SPI_ProcessData_Received()
{
  if (buffer_index >= 4)  // 4 bytes recd from master
  {
    buffer_index = 0;
    
    Mode  = int(buffer_comm[0]);
    data1 = int(buffer_comm[1]);
    data2 = int(buffer_comm[2]);
    data3 = int(buffer_comm[3]);

    number = data1 << 8 | data2;     
    
    return true;
  }
  return false;
}


ISR (SPI_STC_vect) 
{ 
  // SPI interrupt routine 
  buffer_comm[buffer_index] = SPDR; // read byte from SPI Data Register
  buffer_index++;
}
