/*Miscellaneous library file. This file contains diverse useful functions for the RLEG project.
This file implements function definitions only and must be added to the "Inc" folder of the STM32 project.*/

#include "stm32f1xx_hal.h"
#include "string.h"

void write_system_config_to_sd(int FREQ, int PWM_MAX, int PWM_SIGNAL_DURATION, int CURRENT_READ_SAMPLE_TIME, int LOGGING_SAMPLE_TIME); //writes system configuration to data logger

void write_string_to_sd(char string[]); //receives string that is transmitted via UART

void write_current(float current); //writes current readings

int current_sensor_offsetting(); //sensor current offsetting

void pwm_init(void); //pwm initialization

void pwm_set_duty_cycle(int val); //sets pwm dutycycle

void set_dutycycle(float avg_voltage); //calculates dutycyle according to average voltage value

