/*Miscellaneous library file. This file contains diverse useful functions for the RLEG project.
This file implements function definitions only and must be added to the "Inc" folder of the STM32 project.*/

#include "stm32f1xx_hal.h"
#include "string.h"

void write_string_to_sd(char string[]); //receives string that is transmitted via UART

void write_data_to_sd(float acc_data[], float gyro_data[], uint16_t enc_data); //receives sensor data
