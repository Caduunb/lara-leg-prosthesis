#include <rleg.h>

/**
gait_period_based_control.ino
Controls movement based on first gait cycle period. Code starts
counting from the moment the foot hits the ground, then it
calculates one cycle period. For the following periods,
actuation is made based on gait cycle estimation, previous
gait cycle period and pre-defined actuation levels.
**/

#define IMU_MODE (ACCEL_MODE | GYRO_MODE) /* Configuracao de IMU em modo de utilizacao de acelerometro (ACC_MODE) 
                            ou utilizacao de giroscopio (GYRO_MODE)*/
#define VERBOSE_MODE (VERBOSE_XBEE | VERBOSE_USB) /* VERBOSE_USB, VERBOSE_XBEE, VERBOSE_SD: Configuracao para utilizacao de
                      transmissao usb (VERBOSE_USB), xbee(VERSOSE_XBEE) ou cartao SD (VERBOSE_SD)*/

IntervalTimer myTimer;
int acquire = 0;
static char byteRead = 'c';
static char can_start = 0;

int MR_PIN = 3; // Magneto-rheological acting pin
int act_lvl = 0;
float v, k=3;

/********************************** SETUP ***********************************/
void setup()
{
  /** Starts serial communication **/
  Serial.begin(9600);         // Start USB
  //Serial1.begin(9600);         // Start XBee

  rleg.Encoder->begin();      // Start encoder
  rleg.IMU->begin(IMU_MODE);  // Start and configure IMU
  
  myTimer.begin(periodicTask, 10000);
  pinMode(MR_PIN, OUTPUT);
  
  delay(100); // Starting delay
}

#define IMPACT_THRESHOLD 1.8
#define MIN_PERIOD 800
#define MAX_PERIOD 2000
#define K_LR .5 // Constant for Load Response
#define K_TO .8 // Constant for Toe Off
long counter[2] = {0,0};
long period = 0;
byte first_try = -1;
int i = 0;
float Axyz;

/************************************ LOOP ***********************************/
void loop()
{ 
  if(acquire)
  {
    rleg.Encoder->read();             // Read from encoder
    rleg.IMU->read();                 // Read from IMU

    
    /* Calculates acceleration module */
    Axyz = sqrt((rleg.IMU->accel_data[0]) * (rleg.IMU->accel_data[0]) +
                      (rleg.IMU->accel_data[1]) * (rleg.IMU->accel_data[1]) +
                      (rleg.IMU->accel_data[2]) * (rleg.IMU->accel_data[2]));
                      
    /* If acceleration is high enough (sudden change of
       velocity due to impact), program starts counting
       time */
    if(Axyz > IMPACT_THRESHOLD && (millis()-counter[1] > MIN_PERIOD))
    {
      //Serial.print(period);
      //Serial.print(',');
      
      /* Period is calculated */
      counter[0] = counter[1];
      counter[1] = millis();
      period = counter[1] - counter[0];
          
      first_try++;
    }
    
    /* In case it is not the first time, board acts on MR
       based on pre-defined times within period (assuming
       consecutive periods are similar) */
    if(first_try > 0)
    {
      float T_LR_BEG = 0.00*period; // Time for Load Response beginning
      float T_LR_END = 0.10*period; // Time for Load Response ending
      float T_TO_BEG = 0.65*period; // Time for Toe Off beginning
      float T_TO_END = 0.95*period; // Time for Toe Off ending
      
      if(millis()-counter[1] > T_LR_BEG && millis()-counter[1] < T_LR_END)
        act_lvl = rleg.Encoder->encoder_data *1;//* K_LR;  
      else if(millis()-counter[1] > T_TO_BEG && millis()-counter[1] < T_TO_END)
        act_lvl = rleg.Encoder->encoder_data *1;//* K_TO;
      else
        act_lvl = 0;
    }
    
    /* Prints data every five loops, so printing period
       will be sampled accordingly */
    if(i == 5)
    {
      if(Axyz > IMPACT_THRESHOLD)
      {
        Serial.print("IMPACT! "); // debugging
        Serial1.print("IMPACT! "); // debugging
      }
      Serial.print(Axyz); // debugging
      Serial.print(',');
      Serial1.print(Axyz); // debugging
      Serial1.print(',');
      Serial.print(period); // debugging
      Serial.print(',');
      Serial1.print(period); // debugging
      Serial1.print(',');
      rleg.Encoder->show(VERBOSE_MODE); // Show encoder info
      Serial.println(act_lvl);
      Serial1.println(act_lvl);
      
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
