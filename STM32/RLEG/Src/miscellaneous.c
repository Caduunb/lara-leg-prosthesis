/*Miscellaneous STM32 library for useful functions.
This file must be added to the "Src" folder of the STM32 project.*/

#include "miscellaneous.h"
extern UART_HandleTypeDef huart1; //I2C channel
extern TIM_HandleTypeDef htim2;


//data logger functions
void write_string_to_sd(char string[]){ //writes string
	char buffer[50];
	sprintf(buffer, "%s\r\n", string);
	HAL_UART_Transmit(&huart1, (uint8_t *)buffer, strlen(buffer), 100);
}

void write_data_to_sd(float acc_data[], float gyro_data[]){ //writes sensor readings
	char buffer[50];
	sprintf(buffer, "%.2f, %.2f, %.2f, %.2f, %.2f, %.2f\r\n", acc_data[0], acc_data[1], acc_data[2], gyro_data[0], gyro_data[1], gyro_data[2]);
	HAL_UART_Transmit(&huart1, (uint8_t *)buffer, strlen(buffer), 100);
}

//pwm generation functions
void pwm_init(){
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2); // pwm generation on timer2 and channel 2 (PA1 pin)
}

int pwm_set_duty_cycle(int val){
	if(val>0 && val<100){
		TIM2->CCR2 = val; //valor de duty cycle. Config max de acordo com ARR*/
		return 1; //returns 1 for successful setting
	}
	return 0;
}

//current (analog) read functions

