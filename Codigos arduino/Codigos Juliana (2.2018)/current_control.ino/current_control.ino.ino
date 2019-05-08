#include <rleg.h>
#include "utility/TimerOne.h"

/**
  current_control.ino
  Reads current flowing to magneto-rheological
  damp and controls it based on a proportional controller.
  Actuation is performed when velocity is above 
  (in absolute values) a threshold UPPER_EPSILON.
  Once actuation is on, it is turned off only after
  being under a threshold of LOWER_EPSILON.

  Velocity is calculated from encoder readings.

  State 1 corresponds to actuation.
  State 0 corresponds to no actuation.

  The control routine is performed for every 20ms.
 
**/

#define IMU_MODE (ACCEL_MODE) /* (ACCEL_MODE | GYRO_MODE) */
#define VERBOSE_MODE (VERBOSE_USB) /* VERBOSE_USB, VERBOSE_XBEE, VERBOSE_SD */
#define DELTA_T 20 /* calls control routine every 20ms */
#define MAX_ACT_LVL 80 /* for 12V set max lvl */
#define UPPER_EPSILON 40 /* 160 is standard. Units of degrees/sec */
#define LOWER_EPSILON 20 /* 140 is standard. Units of degrees/sec */
#define SIZE_BUFF 100

#define PROP_CONST 0.7 /*proportional gain when actuation is on*/
#define pi 3.1416

int MR_PIN = 3; //magneto-rheological pin

volatile unsigned long time;
unsigned long time_copy;

float k = 0; //proportional constant
float theta_1_copy, abs_v_copy;
int state_copy, act_lvl_copy;
float current_copy;
int max_value_copy, min_value_copy;

volatile int act_lvl = 0;
volatile float v, abs_v;
volatile int state = 0;
volatile float theta_0, theta_1 = 0;

volatile float voltage_rms = 0;
volatile float current_rms = 0;

//current sensor variables
const int sensorIn = A1;
const int sensitivity = 185; // use 185 for 5A, 100 for 20A Module and 66 for 30A Module (units of mV/A)

volatile int readValue;             //value read from the sensor
volatile int max_value = 0;          // store max value here
volatile int min_value = 1024;          // store min value here
volatile float max_current, min_current = 0;
volatile float vp = 0;

unsigned long start;

int count;


/********************************** SETUP ***********************************/
void setup()
{
  // Start serial communication
  Serial.begin(9600);         // Start USB/SD, whichever connected
  rleg.Encoder->begin();      // Start encoder

  //rleg.Encoder->zero();       //zero encoder to adjust offset
  
  Timer1.initialize(30000); //configuration for arduino interrupt
  Timer1.attachInterrupt(controlActuator);
  
  pinMode(MR_PIN, OUTPUT);
  
  delay(100); // Starting delay
}

/************************************ LOOP ***********************************/
void loop()
{ 

  noInterrupts(); //stop interruption to make copies so that variables wont change when buffering
  theta_1_copy = theta_1;
  abs_v_copy = abs_v;
  state_copy = state;
  act_lvl_copy = act_lvl;
  time_copy = time;
  current_copy = current_rms;
  max_value_copy = max_value;
  min_value_copy = min_value;
  interrupts();
  
  writeDataSD(time_copy, theta_1_copy, abs_v_copy, state_copy, act_lvl_copy);
  delay(500);

  start = millis();
    count = 0;
    while(millis() - start <= 5){ //samples for 5ms //pode diminuir pra 3
      readValue = analogRead(sensorIn);
      if (readValue > max_value){
        max_value = readValue;
      }
      else if (readValue < min_value){
        min_value = readValue;
      }
      count++;
    }
  max_current = (max_value*5.0/1023)/sensitivity;
  Serial.print(",");
  min_current = (min_value*5.0/1023)/sensitivity;

   //vp = ((max_value - min_value)*5.0)/1023.0;
  //voltage_rms = vp*sqrt(act_lvl/255); //act_lvl/255 is D, the percentual duty cycle
  //current_rms = (voltage_rms)/sensitivity; //units of mA
}

void controlActuator(){
    rleg.Encoder->read();             // Read from encoder
 
    theta_0 = theta_1; //theta_0 is the previous reading.
    time = millis();
    theta_1 = (rleg.Encoder->encoder_data); // - offset_encoder ); //see if adding offset

    /* Calculates velocity */
    v = (theta_1 - theta_0)/(DELTA_T*0.001); //deltaT is in ms -- v in deg/sec
    abs_v = abs(v); //absolute value

    /* Checks threshold */
    if(abs_v < LOWER_EPSILON && state==1){
      state = 0; // switch to no-actuation state
      k = 0; 
    }

    else if (abs_v > UPPER_EPSILON && state==0){
      state = 1; // switch to proportional actuation state
      k = PROP_CONST;
    }
 
    act_lvl = k*abs_v;
     
    act_lvl = constrain(act_lvl, 0, MAX_ACT_LVL); //act_lvl de 0 a MAX
    
    /* Acts on MR */
    act_lvl = 80;
    analogWrite(MR_PIN,act_lvl);
    //analogWrite(MR_PIN,70);
}

//void currentSensorRead(){
//  int readValue;             //value read from the sensor
//  int maxValue = 0;          // store max value here
//  int minValue = 1024;          // store min value here
//  float vp = 0;
//
//  unsigned long start = millis();
//  while(start - millis()< 3){ //samples for 5ms //pode diminuir pra 3
//    readValue = analogRead(sensorIn);
//    if (readValue > maxValue){
//      maxValue = readValue;
//    }
//    if (readValue < minValue){
//      minValue = readValue;
//    }
//  }
//
//  vp = ((maxValue - minValue)*5.0)/1023.0;
//  voltage_rms = vp*sqrt(act_lvl/255); //act_lvl/255 is D, the percentual duty cycle
//  current_rms = (voltage_rms)/sensitivity; //units of mA
//  
//  //Serial.print("Current: ");  
//  //Serial.print(current_rms);
//  //Serial.print(",");
//}

void writeDataSD(unsigned long time, float theta_1, float abs_v, int state, int act_lvl){

  //Serial.print("t(ms): ");
  Serial.print(time);
  Serial.print(",");
//  Serial.print("enc: ");
//  Serial.print(theta_1);
//  Serial.print(",");
//  Serial.print("vel_abs (gr/s): ");
//  Serial.print(abs_v);  
//  Serial.print(",");
//  Serial.print("state: ");
//  Serial.print(state); 
//  Serial.print(",");
  //Serial.print("act_lvl: ");
  Serial.print(act_lvl);
  Serial.print(",");
  //Serial.print("Current: ");  
  Serial.print(current_rms);
  Serial.print(",");
  Serial.print(max_value); //372
  Serial.print(",");
  Serial.print(min_value); //359
  Serial.print(",");
  Serial.print(max_current);
  Serial.print(",");
  Serial.print(min_current); 
  Serial.print(",");
  Serial.println();

}
