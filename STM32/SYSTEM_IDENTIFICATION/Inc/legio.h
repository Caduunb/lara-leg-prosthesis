/*Miscellaneous library file. This file contains diverse useful functions for the RLEG project.
This file implements function definitions only and must be added to the "Inc" folder of the STM32 project.*/

#include "stm32f1xx_hal.h"
#include "string.h"

void write_string_to_sd(char string[]); //receives string that is transmitted via UART

void write_encoder_data(uint16_t enc_data); //writes encoder readings

void write_current(float current); //writes current readings

void write_data_to_sd(float acc_data[], float gyro_data[], uint16_t enc_data); //receives sensor data

void pwm_init(void);
