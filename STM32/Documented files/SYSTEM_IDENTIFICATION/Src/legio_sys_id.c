/*Miscellaneous STM32 library for useful functions.
This file must be added to the "Src" folder of the STM32 project.*/

#include "legio_sys_id.h"
extern UART_HandleTypeDef huart1; //I2C channel
extern TIM_HandleTypeDef htim2; //PWM channel
extern ADC_HandleTypeDef hadc1; //ADC channel (current analog read)

//data logger (sd card) functions
void write_system_config_to_sd(){ //write system configurations
	//implement here
}

void write_string_to_sd(char string[]){ //writes string
	char buffer[50];
	sprintf(buffer, "%s\r\n", string);
	HAL_UART_Transmit(&huart1, (uint8_t *)buffer, strlen(buffer), 30);
}


void write_current(int current){ //writes current as integer here
	char buffer[50];

	sprintf(buffer, "current: %d\r\n", current);

	HAL_UART_Transmit(&huart1, (uint8_t *)buffer, strlen(buffer), 30);
}

//current sensor functions
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


//pwm functions
void pwm_init(){
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2); // pwm generation on timer2 and channel 2 (PA1 pin)
}

void pwm_set_duty_cycle(int val){
	if(val>=0 && val<100){ //duty cycle in percentage
		TIM2->CCR2 = val; //duty cycle value. Max configuration according to ARR register.
	}
}

