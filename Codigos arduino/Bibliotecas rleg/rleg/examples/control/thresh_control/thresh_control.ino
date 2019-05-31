#include <rleg.h>

/**
  thresh_control.ino
  Controls speed based on encoder readings variation.
  If estimated velocity is above (in absolute values)
  a certain threshold EPSILON, activate magneto-rheological
  damp by a value inversely proportional to difference
  between final angle and current angle.
  
  obs: previous attempts to control velocity by
  proportional actuation were made, but prime goal
  was not achieved, since actuation intervened in movement;
  thus, leg movement was not smooth.
**/

#define IMU_MODE (ACCEL_MODE | GYRO_MODE) // ACCEL_MODE,  GYRO_MODE
#define VERBOSE_MODE (VERBOSE_XBEE)//, VERBOSE_USB)        // VERBOSE_USB, VERBOSE_XBEE, VERBOSE_SD

IntervalTimer myTimer;
int acquire = 0;
static char byteRead = 'c';
static char can_start = 0;

float theta_0, theta_1 = 0;
float EPSILON = 1.5;
int MR_PIN = 3;
int act_lvl = 0;
float v, k=1.3;
int i = 0;
int act = 0;

/********************************** SETUP ***********************************/
void setup()
{
  // Start serial communication
  Serial.begin(9600);         // Start USB
  Serial1.begin(9600);       // Start XBEE

  rleg.Encoder->begin();      // Start encoder
  rleg.IMU->begin(IMU_MODE);  // Start and configure IMU
  
  myTimer.begin(periodicTask, 10000);
  pinMode(MR_PIN, OUTPUT);
  
  delay(100); // Starting delay
}

/************************************ LOOP ***********************************/
void loop()
{ 
  if(acquire)
  {
    // consider using Serial.send_now();
    rleg.Encoder->read();             // Read from encoder
    rleg.IMU->read();                 // Read from IMU
    
    /* Calculates velocity */
    theta_0 = theta_1;
    theta_1 = rleg.Encoder->encoder_data;
    
    v = theta_1 - theta_0;
    
    /* Checks threshold */
    if(v < -EPSILON && !act)
    {
      act = 1;
      act_lvl = 200;
      //      act_lvl = 500/rleg.Encoder->encoder_data;
    }
   
    /* Stops acting on threshold if angle is too small */
    if(rleg.Encoder->encoder_data < 1)
    {
      act = 0;
      act_lvl = 0;
    }
    
    /* Prints data every five loops, so printing period will be sampled
       accordingly */
    if(i == 5)      
    {
      rleg.Encoder->show(VERBOSE_MODE); // Show encoder info
      Serial.print(act_lvl);
      Serial.println();                // Print <enter>
      Serial1.print(act_lvl);
      Serial1.println();                // Print <enter>
      i = 0;
    }
    i++;
    
    /* Acts on MR */
    analogWrite(MR_PIN,act_lvl);
    
    acquire = 0;
  }
}

void periodicTask()
{
  acquire = 1;
}
