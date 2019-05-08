#include <rleg.h>
#include "utility/TimerOne.h"

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

#define IMU_MODE (ACCEL_MODE) // | GYRO_MODE) /*ACCEL_MODE,  GYRO_MODE*/
#define VERBOSE_MODE (VERBOSE_USB) /* VERBOSE_USB)         VERBOSE_USB, VERBOSE_XBEE, VERBOSE_SD*/
#define DELTA_T 20 /*20 ms: tempo de execucao de periodicTask*/
#define MAX_ACT_LVL 256
#define UPPER_EPSILON 40 /*160 //unidades de graus/s de vel angular*/
#define LOWER_EPSILON 20 /*140 //unidades de graus/s de vel angular*/
#define SIZE_BUFF 100

#define PROP_CONST 0.7
#define pi 3.1416

int MR_PIN = 3; //continua sendo o mesmo para teensy e arduino

volatile unsigned long time;
unsigned long time_copy;

const float offset_encoder = -42.82;

float k = 0; //proportional constant
float theta_1_copy, abs_v_copy;
int state_copy, act_lvl_copy;

volatile int act_lvl = 0;
volatile float v, abs_v;
volatile int state = 0;
volatile float theta_0, theta_1 = 0;

/********************************** SETUP ***********************************/
void setup()
{
  // Start serial communication
  Serial.begin(9600);         // Start USB/SD, whichever connected
  rleg.Encoder->begin();      // Start encoder
  
  Timer1.initialize(20000);
  Timer1.attachInterrupt(controlActuator);
  
  pinMode(MR_PIN, OUTPUT);
  
  delay(100); // Starting delay
}

/************************************ LOOP ***********************************/
void loop()
{ 

  noInterrupts(); //stop interruption to make copies so that variable wont change when buffering
  theta_1_copy = theta_1;
  abs_v_copy = abs_v;
  state_copy = state;
  act_lvl_copy = act_lvl;
  time_copy = time;
  interrupts();

//      
//  Serial.print("encoder: ");
//  Serial.print(theta_1_copy);
//  Serial.print(",");
//  Serial.print("vel_abs (graus/s): ");
//  Serial.print(abs_v_copy);
//  Serial.print(",");
//  Serial.print("state: ");
//  Serial.print(state_copy); 
//  Serial.print(",");
//  Serial.print("act_lvl: ");
//  Serial.print(act_lvl_copy); 
//  Serial.println();
  

  writeDataSD(time_copy, theta_1_copy, abs_v_copy, state_copy, act_lvl_copy);
  delay(500);
}


void controlActuator(){
  rleg.Encoder->read();             // Read from encoder
 
    theta_0 = theta_1; //recebe leitura antiga
    time = millis();
    theta_1 = (rleg.Encoder->encoder_data - offset_encoder ); //realiza nova leitura de angulo 
    /* Calculates velocity */
    v = (theta_1 - theta_0)/(DELTA_T*0.001); //deltaT em ms -- v em graus/s
    abs_v = abs(v); //absolute value

    /* Checks threshold */
    if(abs_v < LOWER_EPSILON && state==1){
      state = 0; // chaveia para estado sem atuacao
      k = 0; 
    }

    else if (abs_v > UPPER_EPSILON && state==0){
      state = 1; // chaveia para estado com atuacao proporcional
      k = PROP_CONST;
    }
 
    act_lvl = k*abs_v;
     
    act_lvl = constrain(act_lvl, 0, MAX_ACT_LVL); //act_lvl de 0 a MAX
    
    /* Acts on MR */
    //analogWrite(MR_PIN,act_lvl);
    analogWrite(MR_PIN,70);
}

void writeDataSD(unsigned long time, float theta_1, float abs_v, int state, int act_lvl){

  Serial.print("tempo(ms): ");
  Serial.print(time);
  Serial.print(",");
  Serial.print("encoder: ");
  Serial.print(theta_1);
  Serial.print(",");
  Serial.print("vel_abs (graus/s): ");
  Serial.print(abs_v);  
  Serial.print(",");
  Serial.print("state: ");
  Serial.print(state); 
  Serial.print(",");
  Serial.print("act_lvl: ");
  Serial.print(act_lvl);
  Serial.println();

}
