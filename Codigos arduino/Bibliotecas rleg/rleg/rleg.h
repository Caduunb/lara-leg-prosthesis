#ifndef _RLEG_H
#define _RLEG_H

class IMUClass;
class EncoderClass;

#include <WProgram.h>

//#include <utility/SPI.h>
#include <utility/IMU.h>
#include <utility/Encoder.h>

// Set Teensy clock
#undef  F_CPU
#define F_CPU 16000000L

// Accelerometer thresholds
#define ACC_THRESHOLD1 1
#define ACC_THRESHOLD2 1.5

// IMU Mode
#define ACCEL_MODE 1
#define GYRO_MODE 2

// Print values to Serial
#define VERBOSE_OFF 0
#define VERBOSE_USB 1
#define VERBOSE_XBEE 2
#define VERBOSE_SD 4

// Max knee angle at flexion
#define MAXANGLE 60
#define ACTANGLE 40

// Maximum PWM Level
#define MAXLEVEL 32
#define PWMPin 3

class rlegClass
{
  public:
    /**
     * IMU and Encoder classes (ref: utility/IMU.h, utility/Encoder.h)
     */
    IMUClass* IMU;
    EncoderClass* Encoder;
    
    /**
     * Creates IMU and Encoder classes
     */
    rlegClass();
    
    /**
     * Deletes IMU and Encoder classes
     */
    ~rlegClass();
    
    /**
     * Starts Serial Port(s)
     */
    void startSerial(int verbose_mode);
};

extern rlegClass rleg;

#endif
