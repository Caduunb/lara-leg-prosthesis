#include <rleg.h>

/**
periodic_task.ino
Reads from encoder and IMU in a specific sampling period.
**/

/* Imax = 1.333A @ 8V -> 255
Imax(desired) = (PWM/255)xImax */

#define IMU_MODE (ACCEL_MODE | GYRO_MODE)              // options: ACCEL_MODE,  GYRO_MODE
#define VERBOSE_MODE VERBOSE_USB //(VERBOSE_XBEE | VERBOSE_SD)       // options: VERBOSE_USB, VERBOSE_XBEE, VERBOSE_SD

#define SAMPLING_TIME 10000 // 10ms
long int counter = 0;
IntervalTimer myTimer;
elapsedMicros sincePrint;
int acquire = 0;

/********************************** SETUP ***********************************/
void setup()
{
  // Start serial communication
  Serial.begin(9600);   // Starts USB
  //Serial1.begin(57600);  // Starts XBee
  //Serial3.begin(57600);  // Starts SD
  
  rleg.Encoder->begin();      // Starts encoder
  rleg.IMU->begin(IMU_MODE);  // Starts and configure IMU
  
  /* Sets function periodicTask to timer myTimer,
     so it is called every SAMPLING_TIME microssecond */
  myTimer.begin(periodicTask, SAMPLING_TIME);
  
  delay(100); // Starting delay
}

/************************************ LOOP ***********************************/
void loop()
{   
  /* Tests variable set by periodicTask */
  if(acquire)
  {
    // consider using Serial.send_now();

    rleg.IMU->read();                 // Reads from IMU    
    rleg.Encoder->read();             // Reads from encoder

    rleg.Encoder->show(VERBOSE_MODE); // Shows encoder info
    rleg.IMU->show(VERBOSE_MODE);     // Shows IMU info
    
    //counter++;                      //// ADD THIS FOR TIMESTAMP
    //Serial.print(counter);          ////ADD THIS FOR TIMESTAMP
    
  //  Serial1.print(sincePrint);       //// ADD THIS FOR LOOP PERIOD
  //  Serial3.print(sincePrint);       //// ADD THIS FOR LOOP PERIOD
  //  sincePrint = 0;                 //// ADD THIS FOR LOOP PERIOD
    
    Serial.println();                // Prints <enter> to USB
    //Serial1.println();                // Prints <enter> to XBee
    //Serial3.println();                // Prints <enter> to SD
    
    acquire = 0;
  }
}

void periodicTask()
{
  acquire = 1;
}
