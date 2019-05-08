#include <rleg.h>

/**
sensor_print.ino
Basic printing script. Sampling time is not
programmatically defined.
**/


/**zeroing functions are inseide imu and encoder libraries**/
/* Imax = 1.333A @ 8V -> 255
Imax(desired) = (PWM/255)xImax */

#define IMU_MODE (ACCEL_MODE| GYRO_MODE) //(ACCEL_MODE | GYRO_MODE)
#define VERBOSE_MODE VERBOSE_USB // VERBOSE_USB, VERBOSE_XBEE, VERBOSE_SD
/********************************** SETUP ***********************************/
void setup()
{
  // Start serial communication
  Serial.begin(9600);   // Start USB or SD, whichever connected
 
  rleg.Encoder->begin();      // Start encoder
  rleg.IMU->begin(IMU_MODE);  // Start and configure IMU

  rleg.IMU->zero();           //zero gyroscope to adjust offset
  rleg.Encoder->zero();       //zero encoder to adjust offset
  
  delay(100); // Starting delay
}

/************************************ LOOP ***********************************/
void loop()
{
  rleg.Encoder->read();             // Read from encoder
  delay(100);
  rleg.Encoder->show(VERBOSE_MODE); // Show encoder info //179.47 (quando perna esticada) ,85.17 (joelho dobrado - maximo) // -67 (quando perna esticada) ,26.89 (joelho dobrado - maximo) perna move 94 graua para ambos os casos
  delay(100);
  
  //rleg.IMU->read(IMU_MODE);                 // Read from IMU

  if(IMU_MODE & ACCEL_MODE) //read
  {
    int16_t accel_x, accel_y, accel_z;
    
    rleg.IMU->accel->readAccel( (int*)&accel_x,     // this
                            (int*)&accel_y,     // data
                            (int*)&accel_z  );  // is raw

    // Offsetting
    rleg.IMU->accel_data[0] = (volatile float)accel_x + ACCEL_X_OFFSET;
    rleg.IMU->accel_data[1] = (volatile float)accel_y + ACCEL_Y_OFFSET;
    rleg.IMU->accel_data[2] = (volatile float)accel_z + ACCEL_Z_OFFSET;
    
    // Scaling
    rleg.IMU->accel_data[0] *= ACCEL_X_GAIN;
    rleg.IMU->accel_data[1] *= ACCEL_Y_GAIN;
    rleg.IMU->accel_data[2] *= ACCEL_Z_GAIN;

  }
  
  delay(100);
  
  rleg.IMU->show(VERBOSE_MODE);     // Show IMU info
  delay(100);
  
  Serial.println();                 // Print <enter>
  
  delay(800);                       // Usual delay
}
