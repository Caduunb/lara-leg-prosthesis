/*Miscellaneous library file. This file contains diverse useful functions for the RLEG project.
This file implements function definitions only and must be added to the "Inc" folder of the STM32 project.*/

#include "stm32f1xx_hal.h"
#include "string.h"

void write_system_config_to_sd(); //writes system configuration

void write_string_to_sd(char string[]); //receives string that is transmitted via UART

void write_sampled_data(int time_stamp, int enc_data, float vel, int state, int current); //writes parameters of interest

void pwm_init(); //initializes pwm channel

void pwm_set_duty_cycle(int val); //sets pwm duty cycle

void current_read(); //current sensor read function

int current_sensor_offsetting(); //gets adc value (integer read value) when no current is applied

//led to indicate operating modes functions
void LED_actuation_mode(int state);
