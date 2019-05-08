#include <rleg.h>

/**
sensor_print.ino
Basic printing script. Sampling time is not
programmatically defined.
**/

/* Imax = 1.333A @ 8V -> 255
Imax(desired) = (PWM/255)xImax */

#define IMU_MODE (ACCEL_MODE | GYRO_MODE)         // ACCEL_MODE,  GYRO_MODE
#define VERBOSE_MODE (VERBOSE_USB | VERBOSE_SD)   // VERBOSE_USB, VERBOSE_XBEE, VERBOSE_SD
/********************************** SETUP ***********************************/
void setup()
{
  // Start serial communication
  Serial.begin(9600);   // Start USB
  Serial1.begin(9600);  // Start XBee
  Serial3.begin(9600);  // Start SD
  
  rleg.Encoder->begin();      // Start encoder
  rleg.IMU->begin(IMU_MODE);  // Start and configure IMU
  
  delay(100); // Starting delay
}

/************************************ LOOP ***********************************/
void loop()
{
  rleg.Encoder->read();             // Read from encoder
  rleg.Encoder->show(VERBOSE_MODE); // Show encoder info
  rleg.IMU->read();                 // Read from IMU
  rleg.IMU->show(VERBOSE_MODE);     // Show IMU info
  
  Serial.println();                 // Print <enter> to USB
  Serial1.println();                // Print <enter> to XBee
  Serial3.println();                // Print <enter> to SD
  
  delay(100);                       // Usual delay
}
