/*Miscellaneous STM32 library for useful functions.
This file must be added to the "Src" folder of the STM32 project.*/

#include "legio.h"
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

void write_sampled_data(double time_stamp, int state, uint16_t enc_data, float current){
	char buffer[100];
	sprintf(buffer, "[%f], state: %d, angle: %u, current: %.2f", time_stamp, state, enc_data, current);
	HAL_UART_Transmit(&huart1, (uint8_t *)buffer, strlen(buffer), 100);
}

void write_encoder_data(uint16_t enc_data){ //writes encoder readings
	char buffer[50];
	sprintf(buffer, "angle: %u\r\n", enc_data);
	HAL_UART_Transmit(&huart1, (uint8_t *)buffer, strlen(buffer), 100);
}

void write_current(float current){ //writes encoder readings
	char buffer[50];
	sprintf(buffer, "%.2f \r\n", current);
	HAL_UART_Transmit(&huart1, (uint8_t *)buffer, strlen(buffer), 100);
}

void write_system_config_to_sd(){ //write system configurations

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
void LED_actuation_mode(int state){

	if (state == 0){
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, 0); //yellow led off
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, 1); //red led on means no actuation
		HAL_Delay(5);
	}
	else if (state == 1){
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, 0); //red led off
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, 1); //yellow led means fixed actuation
		HAL_Delay(5);
	}


}
