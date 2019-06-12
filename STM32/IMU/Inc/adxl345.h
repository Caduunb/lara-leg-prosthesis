/*Library file for communicating with accelerometer sensor ADXL345.
This file implements function definitions only and must be added to the "Inc" folder of the STM32 project.*/

#include "stm32f1xx_hal.h"

void adxl_init(void); //sets initial configuration of the device

void adxl_zero_fnc(void); //zeroes the device

void adxl_write_data(uint8_t reg, uint8_t value); //write data to specified register of peripheral device

char adxl_read_reg(uint8_t reg); //reads value from specified register

void adxl_read_data(float *data_acc); //reads acceleration data (in g's)

void adxl_error(void);


