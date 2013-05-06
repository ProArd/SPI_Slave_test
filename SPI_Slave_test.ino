/*
* SPI Slave test program
 * by Rick Anderson
 *
 * Set the defaults:
 * MSBFIRST
 * DataMode = SPI_MODE0;
 * Clock divider = SPI_CLOCK_DIV4,
 */

#include <SPI.h>

const byte TESTBYTE = 0b11110000; 

void setup()
{
  Serial.begin(9600);
  //Slave out needs to be enabled by placing the MISO as OUTPUT
  pinMode(MISO, OUTPUT);

  //Use the AVR Code to turn on slave mode
  SPCR |= _BV(SPE);

  //Standard Arduino settings for SPI
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE0);
  SPI.setClockDivider(SPI_CLOCK_DIV4);

  //Turn on interrupts for SPI
  SPI.attachInterrupt();
  Serial.println("Slave Configured");

}


/*AVR SPI interrupt callback
*Process commands sent to slave
* First transfer is the command value
* Second command pushes the value to the master
*/
ISR (SPI_STC_vect) 
{
  const byte cc = TESTBYTE;
  if (SPDR == 0x00) //Only command is 0x00
  {
    SPDR = 0b11110000; // read byte from SPI Data Register
  }
  else
  {
    SPDR = 0b11111111; //Any other command returns 0xff
  }
}

void loop()
{
  Serial.println("SPI Slave Sketch for testing SPI Master.");
  if (digitalRead (SS) == HIGH)
  {
    SPDR = 0;//When not enable set buffer to 0
  }
}


