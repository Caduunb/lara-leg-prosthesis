
#include <rleg.h>

/**
 * @brief This script reads the data from the IMU and the encoder
 * and drives the current that goes to the MR piston 
 * according to the angular velocity 
 * @authors Cláudia Ochoa, Lucas de Levy
 */

/* Imax = 1.333A @ 8V -> 255
Imax(desired) = (PWM/255)xImax */

#define IMU_MODE (ACCEL_MODE | GYRO_MODE) // ACCEL_MODE,  GYRO_MODE
#define VERBOSE_MODE (VERBOSE_USB | VERBOSE_SD) // VERBOSE_USB, VERBOSE_XBEE, VERBOSE_SD

long int counter = 0;

IntervalTimer myTimer;
elapsedMicros sincePrint;
int acquire = 0;
static char byteRead = 'c';
static char can_start = 0;

/********************************** SETUP ***********************************/
void setup()
{
  // Start serial communication
  rleg.startSerial(VERBOSE_MODE);
  
  rleg.Encoder->begin();      // Start encoder
  rleg.IMU->begin(IMU_MODE);  // Start and configure IMU
  
  myTimer.begin(periodicTask, 10000);
  
  nameSDfile();
  
  delay(100); // Starting delay
}

/************************************ LOOP ***********************************/
void loop()
{ 
  if(acquire)
  {
    // consider using Serial.send_now();
    rleg.Encoder->read();             // Read from encoder
    rleg.Encoder->show(VERBOSE_MODE); // Show encoder info
    rleg.IMU->read();                 // Read from IMU
    rleg.IMU->show(VERBOSE_MODE);     // Show IMU info
    
    //counter++;                 /* ADD THIS FOR TIMESTAMP */
    //Serial.print(sincePrint);  /* ADD THIS FOR LOOP PERIOD */
    //sincePrint = 0;            /* ADD THIS FOR LOOP PERIOD */
        
    acquire = 0;
  }

}

void periodicTask()
{
  acquire = 1;
}

void nameSDfile()
{
  #define NAME_SIZE_MAX 50
  
  char filename[NAME_SIZE_MAX], buff[NAME_SIZE_MAX+20];
  char inChar = 'c';
  int i = 0;
  
  while(Serial.available() > 0)
  {
    if(i < NAME_SIZE_MAX)
    {
      inChar = Serial.read();
      filename[i++] = inChar;
    }
    if (inChar == '\n' || i >= NAME_SIZE_MAX)
    {
      filename[i--] = '\0';
      break;
    }
  }
  
  //Works with Arduino v1.0
  Serial3.write(26);
  Serial3.write(26);
  Serial3.write(26);
  
  while(1)
  {
    if(Serial.available()) 
      if(Serial.read() == '>')
        break;
  }
  
  sprintf(buff, "new %s\r", filename);
  Serial3.print(buff);
  
  while(1)
  {
    if(Serial.available())
      if(Serial.read() == '>')
        break;
  }
  
  sprintf(buff, "append %s\r", filename);
  Serial3.print(buff);
  
  while(1)
  {
    if(Serial.available())
      if(Serial.read() == '<')
        break;
  }
  
  return;
}
