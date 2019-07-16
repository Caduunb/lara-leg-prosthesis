/*Miscellaneous library file. This file contains diverse useful functions for the RLEG project.
This file implements function definitions only and must be added to the "Inc" folder of the STM32 project.*/

#include "stm32f1xx_hal.h"
#include "string.h"

void write_system_config_to_sd(); //writes system configuration to data logger

void write_string_to_sd(char string[]); //receives string that is transmitted via UART

void write_current(int current); //writes current readings

int current_sensor_offsetting(); //sensor current offsetting

void pwm_init(void); //pwm initialization

void pwm_set_duty_cycle(int val); //sets pwm dutycycle

