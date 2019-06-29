/*Miscellaneous STM32 library for useful functions.
This file must be added to the "Src" folder of the STM32 project.*/

#include "legio.h"
extern UART_HandleTypeDef huart1; //I2C channel
extern TIM_HandleTypeDef htim2;
extern ADC_HandleTypeDef hadc1; //ADC channel (current analog read)

//write float function
void write_float(float var, int* int_var, int* dec_var){ //receives variable, integer and decimal parts of variable

	*int_var = (int) var;
	*dec_var = (int) ((var - *int_var)*100); //two decimal places
}

void write_system_config_to_sd(){ //write system configurations

}

//data logger (sd card) functions
void write_string_to_sd(char string[]){ //writes string
	char buffer[50];
	sprintf(buffer, "%s\r\n", string);
	HAL_UART_Transmit(&huart1, (uint8_t *)buffer, strlen(buffer), 30);
}

void write_sampled_data(int time_stamp, int enc_data, float vel, int state, int current){
	char buffer[100];
	int int_vel; //integer and decimal parts of angular velocity
	int dec_vel;

	write_float(vel, &int_vel, &dec_vel);

	if(dec_vel <10){
		sprintf(buffer, "[%d] angle: %d, vel: %d.0%d, state: %d\r\n", time_stamp, enc_data, int_vel, dec_vel, state);
	}
	else{
		sprintf(buffer, "[%d] angle: %d, vel: %d.%d, state: %d\r\n", time_stamp, enc_data, int_vel, dec_vel, state);
	}

	//sprintf(buffer, "[%d] angle: %d, vel: %d, state: %d, current: %.2f", time_stamp, enc_data, state, current);
	//sprintf(buffer, "[%d] angle: %d, state: %d, current: %.2f", time_stamp, enc_data, state, current);
	HAL_UART_Transmit(&huart1, (uint8_t *)buffer, strlen(buffer), 70);
}

//pwm functions
void pwm_init(){
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2); // pwm generation on timer2 and channel 2 (PA1 pin)
}

void pwm_set_duty_cycle(int val){
	if(val>=0 && val<100){ //duty cycle in percentage
		TIM2->CCR2 = val; //duty cycle value. Max configuration according to ARR register.
	}
}

//current sensor read function
void current_read(){

}

int current_sensor_offsetting(){ //gets adc value (integer read value) when no current is applied
	int i;
	int adc_offset_value = 0;

	HAL_ADC_Start(&hadc1); //start adc channel

	HAL_Delay(100); //waits for system stabilization

	for(i=0;i<100;i++){
		HAL_ADC_PollForConversion(&hadc1, 100); //wait for conversion
		adc_offset_value += HAL_ADC_GetValue(&hadc1);
		HAL_Delay(5);
	}

	adc_offset_value = adc_offset_value/100;

	return adc_offset_value;
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
