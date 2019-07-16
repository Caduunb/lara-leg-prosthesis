/*Miscellaneous STM32 library for useful functions.
This file must be added to the "Src" folder of the STM32 project.*/

#include "legio.h"
extern UART_HandleTypeDef huart1; //I2C channel

//data logger (sd card) functions
void write_string_to_sd(char string[]){ //writes string
	char buffer[50];
	sprintf(buffer, "%s\r\n", string);
	HAL_UART_Transmit(&huart1, (uint8_t *)buffer, strlen(buffer), 100);
}

void write_data_to_sd(float enc_pos){ //writes encoder reading
	char buffer[50];

	sprintf(buffer, "%.2f\r\n", enc_pos);
	HAL_UART_Transmit(&huart1, (uint8_t *)buffer, strlen(buffer), 30);
}


