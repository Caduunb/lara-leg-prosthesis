/**
 * @brief Defintions and objects to use the encoder AMT203
 * @authors Cláudia Ochoa, Lucas de Levy
 */

#ifndef _ENCODER_H
#define _ENCODER_H

#include "SPI.h"
#include "../rleg.h"

// Chip select (Teensy)    
// #define CSB_PIN 10
// #define BEGIN_UDELAY 50
// #define BEGIN_DELAY 200
// #define SET_UDELAY 250
// #define GET_UDELAY 50

//Chip select (arduino nano)    
#define CSB_PIN 13
#define BEGIN_UDELAY 50
#define BEGIN_DELAY 200
#define SET_UDELAY 250
#define GET_UDELAY 50

#define ANGLE_OFFSET -8.7000

/**
 * @brief Object to communicate with the encoder
 * @return Actual Position
 */
class EncoderClass
{
  public:
    /**
     * Sensor output (in degrees)
     */
    volatile float encoder_data;
  
    /**
     * Encoder offset
     */
    volatile float encoder_offset;
    char is_offset_ready;
        
    /**
     * Empty constructor
     */
    EncoderClass();
    
    /**
     * Empty destructor
     */
    ~EncoderClass();
    
    /**
     * Starts and configurates SPI
     */
    void begin();
    
    /**
     * Reads from sensor and stores in this->encoder_data
     */
    void read();
    
    /**
     * Prints data in the format "degrees," through USB, XBEE or SD
     */
    void show(int verbose_mode);
    
    /**
     * Returns this->encoder_data
     */
    volatile float getEncoder();
    
    /**
     * Sets encoder offset
     */
    void setOffset();
    void setOffset(volatile float offset_phi);
    
    /**
     * Static zeroes
     */
    void zero();
};

#endif
