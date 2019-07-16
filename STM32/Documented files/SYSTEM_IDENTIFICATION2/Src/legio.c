/*Miscellaneous STM32 library for useful functions.
This file must be added to the "Src" folder of the STM32 project.*/

#include "legio.h"
extern UART_HandleTypeDef huart1; //I2C channel
extern TIM_HandleTypeDef htim2;
extern ADC_HandleTypeDef hadc1; //ADC channel (current analog read)

#define PWM_MAX 12

//data logger (sd card) functions

void write_sampled_data(int v_avg, int dutycycle, int adc_value){
	char buffer[100];

	sprintf(buffer, "v_avg: %d, dutycycle: %d, adc_value: %d\r\n", v_avg, dutycycle, adc_value);

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

void set_avg_voltage(float v_avg, int* dutycycle){ //configures the pwm dutycycle parameter based on specified avg voltage
	*dutycycle = (int)((v_avg/PWM_MAX)*100); //duty cycle as percentage. PWM_MAX == pwm amplitude, aprox 12V
	pwm_set_duty_cycle(*dutycycle); //sets duty cycle as an integer
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

