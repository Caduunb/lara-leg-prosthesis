/**
 * @headfile Encoder.h "Encoder.h"
 */

#include "Encoder.h"

EncoderClass::EncoderClass()
{
}

EncoderClass::~EncoderClass()
{
}

void EncoderClass::begin()
{
  pinMode(CSB_PIN,OUTPUT); //Slave Select
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE0);

  SPI.begin();

  digitalWrite(CSB_PIN, HIGH); //start with cs high

  //Cleaning out the buffer
  for (int i = 0; i < 3; i++)
  {
    digitalWrite(CSB_PIN, LOW);
    SPI.transfer(0x00);
    digitalWrite(CSB_PIN, HIGH);
    delayMicroseconds(BEGIN_UDELAY);
  }

  this->is_offset_ready = 0;

  // Initialization delay (initially 5000)
  delay(BEGIN_DELAY);
}

void EncoderClass::read()
{
  float angle = 0;

  digitalWrite(CSB_PIN, HIGH); //start with cs high

  // delay(250);

  //I use this to clean out the buffer for peace of mind
  for (int i = 0; i < 3; i++)
  {
    digitalWrite(CSB_PIN, LOW);
    SPI.transfer(0x00);
    digitalWrite(CSB_PIN, HIGH);
    delayMicroseconds(SET_UDELAY);
  }
  
  //rd_pos
  digitalWrite(CSB_PIN, LOW);
  SPI.transfer(0x10);
  digitalWrite(CSB_PIN, HIGH);
  delayMicroseconds(SET_UDELAY); //not sure why, but in my playing with this it needed this delay in here

  //nop
  digitalWrite(CSB_PIN, LOW);
  SPI.transfer(0x00);
  digitalWrite(CSB_PIN, HIGH);
  delayMicroseconds(SET_UDELAY); //not sure why, but in my playing with this it needed this delay in here

  //run check for nop and response
  digitalWrite(CSB_PIN, LOW);
  while (SPI.transfer(0x00) == 0xa5); //continue sending nop commants until returned value is not 0xa5...could also say, wait until command returned is 0x10
  digitalWrite(CSB_PIN, HIGH);
  delayMicroseconds(GET_UDELAY);

  //get high value
  digitalWrite(CSB_PIN, LOW);
  float MSB_degrees = SPI.transfer(0x00);
  digitalWrite(CSB_PIN, HIGH);
  delayMicroseconds(GET_UDELAY);

  //get low value
  digitalWrite(CSB_PIN, LOW);
  float LSB_degrees = SPI.transfer(0x00); //low byte displayed as binary
  digitalWrite(CSB_PIN, HIGH);
  delayMicroseconds(GET_UDELAY);
  
  angle = (((MSB_degrees*256) + LSB_degrees)*360)/4096.;
  angle = 360 - angle;
  angle = angle;
  if(angle > 180) angle -= 360;

  if(this->is_offset_ready == 1)
    this->encoder_data = angle + this->encoder_offset;
  else
    this->encoder_data = angle;
}

void EncoderClass::show(int verbose_mode)
{
  if(verbose_mode & VERBOSE_USB)
  {
    Serial.print(this->encoder_data);
    Serial.print(",");
  }
  if(verbose_mode & VERBOSE_XBEE)
  {
    Serial1.print(this->encoder_data);
    Serial1.print(",");
  }
  if(verbose_mode & VERBOSE_SD)
  {
    Serial3.print(this->encoder_data);
    Serial3.print(",");
  }
}

void EncoderClass::setOffset()
{
  this->encoder_offset = -this->encoder_data;
}

void EncoderClass::setOffset(volatile float encoder_offset)
{
  this->encoder_offset = -encoder_offset;
}

void EncoderClass::zero()
{  
  #define NUM_READINGS 10
  char byteRead = 'c';
  volatile float offset_phi = 0;
  
  Serial.print("Zero encoder?<Z>");
  while(this->is_offset_ready == 0)
  {
    if(Serial.available() > 0)
    {
      byteRead = Serial.read();
      if(byteRead == 'z' || byteRead == 'Z')
        is_offset_ready = 1;
    }
    if(this->is_offset_ready == 1)
    {
      Serial.print("\tZeroing...");
      delay(100);
      this->is_offset_ready = 0;
      for(int i = 0; i < NUM_READINGS; i++)
      {
        this->read();
        offset_phi += this->encoder_data;
        delay(100);
      }
      this->is_offset_ready = 1;
      this->setOffset(offset_phi/NUM_READINGS);
      delay(100);
      Serial.println("\tZeroed!");
    }
    
    delay(100);
  }
}
