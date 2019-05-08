/**
 * @headfile IMU.h "IMU.h"
 */

#include "IMU.h"

IMUClass::IMUClass()
{
  //this->accel = new ADXL345();  // consider adding this
  //this->gyro = new ITG3200();
}

IMUClass::~IMUClass()
{
 // delete this->accel;           // consider adding this
 // delete this->gyro;
}

void IMUClass::begin(int mode)
{
  Wire.begin();
  
  // Sets the IMU mode (ACCE_MODE, GYRO_MODE, ACCEL_MODE | GYRO_MODE...)
  this->imu_mode = mode;

  if(mode & ACCEL_MODE)
  {
    this->accel = new ADXL345();  // consider removing this
    this->accel->powerOn();
    this->accel->setFullResBit(true);
  }
  if(mode & GYRO_MODE)
  {
    this->gyro = new ITG3200();   // consider removing this
    this->gyro->begin(0x68);
    
    this->gyro_offsets[0] = GYRO_X_OFFSET;
    this->gyro_offsets[1] = GYRO_Y_OFFSET;
    this->gyro_offsets[2] = GYRO_Z_OFFSET;
    
    is_offset_ready = 0;
  }
}

void IMUClass::read()
{
  if(imu_mode & ACCEL_MODE)
  {
    int16_t accel_x, accel_y, accel_z;
    
    this->accel->readAccel( (int*)&accel_x,     // this
                            (int*)&accel_y,     // data
                            (int*)&accel_z  );  // is raw

    // Offsetting
    this->accel_data[0] = (volatile float)accel_x + ACCEL_X_OFFSET;
    this->accel_data[1] = (volatile float)accel_y + ACCEL_Y_OFFSET;
    this->accel_data[2] = (volatile float)accel_z + ACCEL_Z_OFFSET;
    
    // Scaling
    this->accel_data[0] *= ACCEL_X_GAIN;
    this->accel_data[1] *= ACCEL_Y_GAIN;
    this->accel_data[2] *= ACCEL_Z_GAIN;

  }
  if(imu_mode & GYRO_MODE)
  {
    int16_t gyro_x, gyro_y, gyro_z;
    
    gyro_x = this->gyro->getX();  // this data
    gyro_y = this->gyro->getY();  // data
    gyro_z = this->gyro->getZ();  // is raw
    
    //Serial.print("DENTRO DO LOOP: ");
    //Serial.print(gyro_x);
   // Serial.print(", ");
    //Serial.print("[[");
    //Serial.print(gyro_offsets[0]);  // PRINT NECESSARIO 1
    //Serial.print("\t");

    // Offsetting
    if(is_offset_ready)
    {
      this->gyro_data[0] = (volatile float)gyro_x + gyro_offsets[0];
      this->gyro_data[1] = (volatile float)gyro_y + gyro_offsets[1];
      this->gyro_data[2] = (volatile float)gyro_z + gyro_offsets[2];
    }
    else
    {
      this->gyro_data[0] = (volatile float)gyro_x;
      this->gyro_data[1] = (volatile float)gyro_y;
      this->gyro_data[2] = (volatile float)gyro_z;
    }
    
    //Serial.print("\tOBJETIVO1: ");
    //Serial.print(gyro_data[0]);
    //Serial.print("\t");
    
    // Scaling
    this->gyro_data[0] *= GYRO_X_GAIN;
    this->gyro_data[1] *= GYRO_Y_GAIN;
    this->gyro_data[2] *= GYRO_Z_GAIN;
    
    //Serial.print("\tOBJETIVO2\t");
    //Serial.print(gyro_data[0]);   // PRINT NECESSARIO 2
    //Serial.print("]]");
    //Serial.print("\t");
  }
}

void IMUClass::show(int verbose_mode)
{
  if(verbose_mode & VERBOSE_USB)
  {
    if(this->imu_mode & ACCEL_MODE)
    {
      Serial.print(this->accel_data[0]);
      Serial.print(",");
      Serial.print(this->accel_data[1]);
      Serial.print(",");
      Serial.print(this->accel_data[2]);
      Serial.print(",");
    }
    if(this->imu_mode & GYRO_MODE)
    {
      Serial.print(this->gyro_data[0]);
      Serial.print(",");
      Serial.print(this->gyro_data[1]);
      Serial.print(",");
      Serial.print(this->gyro_data[2]);
      Serial.print(",");
    }
  }
  if(verbose_mode & VERBOSE_XBEE)
  {
    if(this->imu_mode & ACCEL_MODE)
    {
      Serial1.print(this->accel_data[0]);
      Serial1.print(",");
      Serial1.print(this->accel_data[1]);
      Serial1.print(",");
      Serial1.print(this->accel_data[2]);
      Serial1.print(",");
    }
    if(this->imu_mode & GYRO_MODE)
    {
      Serial1.print(this->gyro_data[0]);
      Serial1.print(",");
      Serial1.print(this->gyro_data[1]);
      Serial1.print(",");
      Serial1.print(this->gyro_data[2]);
      Serial1.print(",");
    }
  }
  if(verbose_mode & VERBOSE_SD)
  {
    if(this->imu_mode & ACCEL_MODE)
    {
      Serial3.print(this->accel_data[0]);
      Serial3.print(",");
      Serial3.print(this->accel_data[1]);
      Serial3.print(",");
      Serial3.print(this->accel_data[2]);
      Serial3.print(",");
    }
    if(this->imu_mode & GYRO_MODE)
    {
      Serial3.print(this->gyro_data[0]);
      Serial3.print(",");
      Serial3.print(this->gyro_data[1]);
      Serial3.print(",");
      Serial3.print(this->gyro_data[2]);
      Serial3.print(",");
    }
  }
}

void IMUClass::setMode(int mode)
{
  this->imu_mode = mode;
}

void IMUClass::setGyroOffsets()
{
  this->gyro_offsets[0] = -this->gyro_data[0];
  this->gyro_offsets[1] = -this->gyro_data[1];
  this->gyro_offsets[2] = -this->gyro_data[2];
}

void IMUClass::setGyroOffsets(volatile float offset_x, volatile float offset_y, volatile float offset_z)
{
  this->gyro_offsets[0] = -offset_x;
  this->gyro_offsets[1] = -offset_y;
  this->gyro_offsets[2] = -offset_z;
}

void IMUClass::zero()
{  
  #define NUM_READINGS 100
  char byteRead = 'c';
  volatile float offset_x = 0, offset_y = 0, offset_z = 0;
  
  Serial.print("Zero IMU? <Z>");
  while(is_offset_ready == 0)
  {
    if(Serial.available() > 0)
    {
      byteRead = Serial.read();
      if(byteRead == 'z' || byteRead == 'Z')
        is_offset_ready = 1;
    }
    if(is_offset_ready == 1)
    {
      Serial.print("\tZeroing...");
      delay(100);
      this->is_offset_ready = 0;
      for(int i = 0; i < NUM_READINGS; i++)
      {
        this->read();
        offset_x += this->gyro_data[0];
        offset_y += this->gyro_data[1];
        offset_z += this->gyro_data[2];
        delay(10);
      }
      this->is_offset_ready = 1;
      this->setGyroOffsets(offset_x/NUM_READINGS, offset_y/NUM_READINGS, offset_z/NUM_READINGS);
      delay(100);
      Serial.println("\tZeroed!");
    }
    
    delay(100);
  }
}
