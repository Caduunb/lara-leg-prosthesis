/**
 * @headfile rleg.h ""
 */

#include "rleg_arduino.h"

rlegClass rleg;

rlegClass::rlegClass()
{
  this->IMU = new IMUClass;
  this->Encoder = new EncoderClass;
}

rlegClass::~rlegClass()
{
  delete this->IMU;
  delete this->Encoder;
}

void rlegClass::startSerial(int verbose_mode)
{
  if(verbose_mode & VERBOSE_USB)
  {
    Serial.begin(9600);
  }
  if(verbose_mode & VERBOSE_XBEE)
  {
    Serial2.begin(9600);
  }
  if(verbose_mode & VERBOSE_SD)
  {
    Serial3.begin(9600);
  }
}
