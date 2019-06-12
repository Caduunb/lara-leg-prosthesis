/*Library file for communicating with ITG3200 gyroscope sensor.
This file implements function definitions only and must be added to the "Inc" folder of the STM32 project.*/

#include "stm32f1xx_hal.h"

void itg_init_no_interrupt(void); //sets initial configuration of the device in standard mode

void itg_init_set_interrupt(void); //sets initial configuration of the device in interruption mode

void itg_zero_fnc(void); //zeroes the device

void itg_write_data(uint8_t reg, uint8_t value);//write to register to set configurations

char itg_read_reg(uint8_t reg); //reads value from specified register

void itg_read_data_no_interrupt(float *gyro_data); //reads gyro data in stardard mode

void itg_read_data_when_ready(float *gyro_data);  //reads gyro data in interrupt mode (reads when new data is available)

void itg_error(void);


