/*Miscellaneous STM32 library for useful functions.
This file must be added to the "Src" folder of the STM32 project.*/

#include "miscellaneous.h"
extern UART_HandleTypeDef huart1; //I2C channel
extern TIM_HandleTypeDef htim2;

//data logger (sd card) functions
void write_string_to_sd(char string[]){ //writes string
	char buffer[50];
	sprintf(buffer, "%s\r\n", string);
	HAL_UART_Transmit(&huart1, (uint8_t *)buffer, strlen(buffer), 100);
}

void write_data_to_sd(float acc_data[], float gyro_data[], uint16_t enc_data){ //writes sensor readings
	char buffer[50];
	sprintf(buffer, "%.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %u\r\n", acc_data[0], acc_data[1], acc_data[2], gyro_data[0], gyro_data[1], gyro_data[2], enc_data);
	HAL_UART_Transmit(&huart1, (uint8_t *)buffer, strlen(buffer), 100);
}

void write_system_config_to_sd(){ //write system

}

//pwm functions
void pwm_init(){
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2); // pwm generation on timer2 and channel 2 (PA1 pin)
}

void pwm_set_duty_cycle(int val){
	if(val>0 && val<100){ //duty cycle in percentage
		TIM2->CCR2 = val; //duty cycle value. Max configuration according to ARR register.
	}
}

//current sensor read function
void current_read(){

}

//led to indicate operating modes functions
void set_no_actuation_mode(){
	//red led on
	//print to sd configuration
}
