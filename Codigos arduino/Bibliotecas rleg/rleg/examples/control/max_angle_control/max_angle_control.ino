#include <rleg.h>

/**
gait_period_based_control.ino
Controls movement based on first gait cycle period. Code starts
counting from the moment the foot hits the ground, then it
calculates one cycle period. For the following periods,
actuation is made based on gait cycle estimation, previous
gait cycle period and pre-defined actuation levels.
**/

#define IMU_MODE (ACCEL_MODE | GYRO_MODE) // ACCEL_MODE,  GYRO_MODE
#define VERBOSE_MODE (VERBOSE_XBEE, VERBOSE_USB)        // VERBOSE_USB, VERBOSE_XBEE, VERBOSE_SD

IntervalTimer myTimer;
int acquire = 0;
static char byteRead = 'c';
static char can_start = 0;

int MR_PIN = 3;
int act_lvl = 0;
float v, k=3;

/********************************** SETUP ***********************************/
void setup()
{
  /** Starts serial communication **/
  Serial.begin(9600);         // Start USB

  rleg.Encoder->begin();      // Start encoder
  rleg.IMU->begin(IMU_MODE);  // Start and configure IMU
  
  myTimer.begin(periodicTask, 10000);
  pinMode(MR_PIN, OUTPUT);
  
  delay(100); // Starting delay
}

#define IMPACT_THRESHOLD 1.8
#define MAX_ANGLE 30
#define MIN_ANGLE 3
#define MAX_ACT 1000
#define BEFORE_SWING 0
#define ON_SWING 1
long counter[2] = {0,0};
long period = 0;
byte first_point = 1;
byte second_point = 1;
long angle[2] = {0,0};
long velocity = 0;
int i = 0;
float Axyz;
byte current_phase = BEFORE_SWING;

/************************************ LOOP ***********************************/
void loop()
{ 
  if(acquire)
  {
    rleg.Encoder->read();             // Read from encoder
    rleg.IMU->read();                 // Read from IMU
    
    angle[1] = angle[0];
    angle[0] = rleg.Encoder->encoder_data;
    
    velocity = angle[1] - angle[0];

    switch(current_phase)
    {
      case BEFORE_SWING:
        /* Calculates acceleration module */
        Axyz = sqrt((rleg.IMU->accel_data[0]) * (rleg.IMU->accel_data[0]) +
                      (rleg.IMU->accel_data[1]) * (rleg.IMU->accel_data[1]) +
                      (rleg.IMU->accel_data[2]) * (rleg.IMU->accel_data[2]));
                      
        /* If impact is high enough, start counting */
        if(first_point && Axyz > IMPACT_THRESHOLD)
        {      
          counter[0] = millis();
          first_point = 0;
        }
        
        /* If maximum angle is achieved,
           stop counting and calculate period */
        if(second_point && angle[0] >= MAX_ANGLE)
        {      
          counter[1] = millis();      
          second_point = 0;
          
          /* Period is calculated */
          period = counter[1] - counter[0];
        }
        
        /* Whenever maximum angle is achieved,
           hold the leg */
        if(angle[0] >= MAX_ANGLE)
          act_lvl = MAX_ACT;
        /* Next phase transition: in case angle is less
           than minimum and velocity is negative */
        else if(angle[0] <= MIN_ANGLE && velocity < -1)
        {
          current_phase = ON_SWING;
          first_point = 1;
          second_point = 1;
        }
        /* If angle is less than maximum, don't hold */
        else
          act_lvl = 0;
          
      break;
      
      case ON_SWING:
        /** Add acting profile here based on "period" **/
      
        /* Next phase transition: in case angle is less
           than minimum and velocity is negative */
        if(angle[0] <= MIN_ANGLE && velocity < -1)
          current_phase = BEFORE_SWING;  
      break;
    }
    
    /* Prints data every five loops, so printing period
    will be sampled accordingly */
    if(i == 5)
    {
      rleg.Encoder->show(VERBOSE_MODE); // Show encoder info
      Serial.print(act_lvl);
      Serial.println();                // Print <enter>
      i = 0;
    }
    i++;
    
    analogWrite(MR_PIN,act_lvl);
    
    acquire = 0;
  }
}

void periodicTask()
{
  acquire = 1;
}
