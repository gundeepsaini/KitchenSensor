

void Comm_SPI_Config()
{
   digitalWrite(SS, HIGH);
   
   SPI.begin();

   int speedMaximum = 1000000;   // 1 Mhz
   SPI.beginTransaction(SPISettings(speedMaximum, MSBFIRST , SPI_MODE0)); 
   
   //SPI.setClockDivider(SPI_CLOCK_DIV128);
}



void Send_data_SPI(byte a1, byte a2, byte a3)
{
   byte ReservedByte = 0;

   digitalWrite(SS, LOW); 
   SPI.transfer(a1);
   SPI.transfer(a2);
   SPI.transfer(a3);
   SPI.transfer(ReservedByte);
   digitalWrite(SS, HIGH);
}


void Send_data_SPI(byte a1, int IntegerNumber)
{
   byte ReservedByte = 0;
   byte a2, a3;

   a2 = highByte(IntegerNumber);
   a3 = lowByte(IntegerNumber);

   digitalWrite(SS, LOW); 
   SPI.transfer(a1);
   SPI.transfer(a2);
   SPI.transfer(a3);
   SPI.transfer(ReservedByte);
   digitalWrite(SS, HIGH);
}
