#ifndef _IMU_H
#define _IMU_H

#include "Wire.h" 
#include "ADXL345.h"
#include "ITG3200.h"

#include "../rleg.h"

// Accel offsets and gains
#define ACCEL_X_OFFSET  -11.
#define ACCEL_Y_OFFSET  6.
#define ACCEL_Z_OFFSET  21.

#define ACCEL_X_GAIN  1./250.1892
#define ACCEL_Y_GAIN  1./269.8986
#define ACCEL_Z_GAIN  1./284.2727

// Gyro offsets
#define GYRO_X_OFFSET  (volatile float)(+5.3333*14.375)
#define GYRO_Y_OFFSET  (volatile float)(-2.47210*14.375)
#define GYRO_Z_OFFSET  (volatile float)(-0.1443*14.375)

#define GYRO_X_GAIN  1./14.375
#define GYRO_Y_GAIN  1./14.375
#define GYRO_Z_GAIN  1./14.375

class IMUClass
{
  public:
    /**
     * Operation mode: logically OR modes to combine
     * ACCEL_MODE
     * GYRO_MODE
     * ex: ACCEL_MODE | GYRO_MODE
     */
    volatile int imu_mode;
    
    /**
     * Accelerometer and Gyroscope classes (ref: ADXL345.h, itg3200.h)
     */
    ADXL345* accel;
    ITG3200* gyro;
    
    /**
     * Sensors outputs (in gravities and degrees/second)
     */
    volatile float accel_data[3];
    volatile float gyro_data[3];
    
    /**
     * Gyrometer offsets
     */
    volatile float gyro_offsets[3];
    char is_offset_ready;
    
    /**
     * Empty constructor
     */
    IMUClass();
    
    /**
     * Empty destructor
     */
    ~IMUClass();  
  
    /**
     * Starts and configurates I2C
     * (no delay)
     */
    void begin(int mode);
    
    /**
     * Reads from sensors and
     * stores in this->accel_data and/or this->gyro_data
     * (no delay)
     */
    void read();
    
    /**
     * Prints data in the format "data_x,data_y,data_z"
     * through USB, XBEE or SD
     * (no delay)
     */
    void show(int verbose_mode);
    
    /**
     * Returns this->accel_data / this->gyro_data
     */
    volatile float* getAccel();
    volatile float* getGyro();
    
    /**
     * Sets this->imu_mode
     */
    void setMode(int mode);
    
    /**
     * Sets gyrometer offsets
     */
    void setGyroOffsets();
    void setGyroOffsets(volatile float offset_x, volatile float offset_y, volatile float offset_z);

    /**
     * Static zeroes
     */
    void zero();

};

#endif
