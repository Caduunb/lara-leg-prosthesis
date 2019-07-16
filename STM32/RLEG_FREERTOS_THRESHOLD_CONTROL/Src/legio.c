/*Input/Output RLEG library for STM32. Implements useful functions.
This file must be added to the "Src" folder of the STM32 project.*/

#include "legio.h"
extern UART_HandleTypeDef huart1; //I2C channel
extern TIM_HandleTypeDef htim2; //timer for PWM generation

//write float function
void write_float(float var, int* int_var, int* dec_var){ //receives variable, integer and decimal parts of variable
	*int_var = (int) var;
	*dec_var = (int) ((var - *int_var)*100); //two decimal places
}

void write_system_config_to_sd(){ //write system configurations
  //add data here as needed and transmit via UART when program starts. Ex: clock_speed, sampled_data, sample_time, pwm frequency, etc
}

//data logger (sd card) functions

void write_string_to_sd(char string[]){ //writes string to SD card
	char buffer[50]; //buffer string
	sprintf(buffer, "%s\r\n", string); //put string into buffer
	HAL_UART_Transmit(&huart1, (uint8_t *)buffer, strlen(buffer), 30); //transmit buffer. 30 ms timeout
}

void write_sampled_data(int enc_data, float vel, int state){ //writes sampled data to SD card
	char buffer[100];
	int int_vel; //integer and decimal parts of angular velocity
	int dec_vel;

	write_float(vel, &int_vel, &dec_vel); //use this function to separate float vel into integer and decimal parts with 2 decimal digits
	//--> function that writes string into buffer via sprintf to transmit via UART to SD card doenst work with FreeRTOS.
	//therefore, the calculated angular velocity is written as a float and the other variables, as integers.

	if(dec_vel <10){
		sprintf(buffer, "angle: %d, vel: %d.0%d, state: %d\r\n", enc_data, int_vel, dec_vel, state);
	}
	else{
		sprintf(buffer, "angle: %d, vel: %d.%d, state: %d\r\n", enc_data, int_vel, dec_vel, state);
	}

	HAL_UART_Transmit(&huart1, (uint8_t *)buffer, strlen(buffer), 70); //transmit via UART with 70 ms timeout
}

//pwm functions
void pwm_init(){
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2); // pwm generation on timer2 and channel 2 (PA1 pin)
}

void pwm_set_duty_cycle(int val){
	if(val>=0 && val<100){ //duty cycle in percentage. According to CubeMX set configuration, duty cyle is changed in steps of 1
		TIM2->CCR2 = val; //duty cycle value. Max configuration according to ARR register set in the CubeMX project
	}
}

//led to indicate operating modes functions
void LED_actuation_mode(int state){ //test this

	if (state == 0){
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, 0); //yellow led off
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, 1); //red led on means no actuation
		HAL_Delay(1);
	}
	else if (state == 1){
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, 0); //red led off
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, 1); //yellow led means fixed actuation
		HAL_Delay(1);
	}


}
