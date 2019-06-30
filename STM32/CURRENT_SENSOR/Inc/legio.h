/*Miscellaneous library file. This file contains diverse useful functions for the RLEG project.
This file implements function definitions only and must be added to the "Inc" folder of the STM32 project.*/

#include "stm32f1xx_hal.h"
#include "string.h"

void get_pwm_frequency(); //get pwm frequency

void write_pwm_freq_to_sd(float frequency, char string[]);

void write_sampled_data_current_sensor(int v_avg, int dutycyle, int adc_value);

void pwm_init(); //initializes pwm channel

void pwm_set_duty_cycle(int val); //sets pwm duty cycle

void set_avg_voltage(float v_avg, int* dutycycle); //sets average voltage value

void current_read(); //current sensor read function

int current_sensor_offsetting(); //gets adc value (integer read value) when no current is applied

