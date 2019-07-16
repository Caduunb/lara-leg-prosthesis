/*Library file for communicating with AMT20 encoder.
This file implements function definitions only and must be added to the "Inc" folder of the STM32 project.*/
#include "legio.h"
#include "stm32f1xx_hal.h"

char encoder_transmit_command(uint8_t command); //transmit a command to execute action and get response from sensor

char encoder_get_data(uint8_t command); //transmit a command and receive read data back

void encoder_init(void); //encoder initialization

int encoder_set_zero_pos(void); //zeroes sensor position

void encoder_read_pos(float *position); //reads sensor position
