#include <rleg.h>

/**
zero_and_start.ino
Waits for command from user ('z' or 'Z') to zero reading values.
Waits for another command from user ('s' or 'S') to start acquiring
data at programmatically defined sampling time, pausing
whenever user sends same command ('s' or 'S').
**/

/* Imax = 1.333A @ 8V -> 255
Imax(desired) = (PWM/255)xImax */

#define IMU_MODE (ACCEL_MODE | GYRO_MODE) // ACCEL_MODE,  GYRO_MODE
#define VERBOSE_MODE (VERBOSE_USB | VERBOSE_SD)        // VERBOSE_USB, VERBOSE_XBEE, VERBOSE_SD

long int counter = 0;

#define SAMPLING_PERIOD 10000
IntervalTimer myTimer;
elapsedMicros sincePrint;
int acquire = 0;
static char byteRead = 'c';
static char can_start = 0;

/********************************** SETUP ***********************************/
void setup()
{
  // Starts serial communication
  Serial.begin(9600);   // Starts USB

  rleg.Encoder->begin();      // Starts encoder
  rleg.IMU->begin(IMU_MODE);  // Starts and configures IMU
  
  /* Runs zero routine (present in library) */
  if(can_start == 0)
  {
    delay(1000);
    rleg.IMU->zero();
    rleg.Encoder->zero();
    can_start = start();
    can_start = 1;
  }
  
  myTimer.begin(periodicTask, SAMPLING_PERIOD);
  
  delay(100); // Starting delay
}

/************************************ LOOP ***********************************/
void loop()
{ 
  /* Start only if periodicTask was called and start() function was run */
  if(acquire && can_start)
  {
    // consider using Serial.send_now();
    rleg.Encoder->read();             // Read from encoder
    rleg.Encoder->show(VERBOSE_MODE); // Show encoder info
    rleg.IMU->read();                 // Read from IMU
    rleg.IMU->show(VERBOSE_MODE);     // Show IMU info
    
    //counter++;                      //// ADD THIS FOR TIMESTAMP
    //Serial.print(sincePrint);       //// ADD THIS FOR LOOP PERIOD
    //sincePrint = 0;                 //// ADD THIS FOR LOOP PERIOD
    //Serial.print(", "); 
    //Serial.print(counter);          //// ADD THIS FOR TIMESTAMP
    //Serial.print(", "); 
    //Serial.print(rleg.Encoder->encoder_offset);
    //Serial.print(", "); 
    //Serial.print(rleg.IMU->gyro_offsets[0]/14.375);
    //Serial.print(", "); 
    //Serial.print(rleg.IMU->gyro_offsets[1]/14.375);
    //Serial.print(", "); 
    //Serial.print(rleg.IMU->gyro_offsets[2]/14.375);

    Serial.println();                // Prints <enter>
    
    acquire = 0;
  }
  
  /* Acquiring routine is disabled once 's'
     character is sent */
  if(Serial.available() > 0)
  {
    byteRead = Serial.read();
    if(byteRead == 's' || byteRead == 'S')
      can_start = !can_start;
  }
}

void periodicTask()
{
  acquire = 1;
}

/* Starting routine. Waits until user sends 's' character. */
char start()
{
  char start = 0;
  
  Serial.print("Start?<S> ");
  while(start == 0)
  {
    if(Serial.available() > 0)
    {
      byteRead = Serial.read();
      if(byteRead == 's' || byteRead == 'S')
        start = 1;
    }
    
    if(start == 1)
      //Serial.println("\tStart!");
    
    delay(100);
  }
  
  return start;
}
