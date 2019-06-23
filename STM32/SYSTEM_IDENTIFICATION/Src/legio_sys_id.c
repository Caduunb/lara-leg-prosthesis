/*Miscellaneous STM32 library for useful functions.
This file must be added to the "Src" folder of the STM32 project.*/

#include "legio_sys_id.h"
extern UART_HandleTypeDef huart1; //I2C channel
extern TIM_HandleTypeDef htim2; //PWM channel
extern ADC_HandleTypeDef hadc1; //ADC channel (current analog read)



//data logger (sd card) functions
void write_system_config_to_sd(int FREQ, int PWM_MAX, int PWM_SIGNAL_DURATION, int CURRENT_READ_SAMPLE_TIME, int LOGGING_SAMPLE_TIME){ //write system configurations //change this later
	char buffer[200];

	HAL_Delay(100);

	sprintf(buffer, "Piston response identification routine\r\n\r\n");
	HAL_UART_Transmit(&huart1, (uint8_t *)buffer, strlen(buffer), 100);
	sprintf(buffer, "System Configurations: pwm frequency @%d kHz, %d V battery, %d ms of pwm signal duration, %d ms current sample time and %d ms logging sample time\r\n\r\n", FREQ, PWM_MAX, PWM_SIGNAL_DURATION, CURRENT_READ_SAMPLE_TIME, LOGGING_SAMPLE_TIME);
	HAL_UART_Transmit(&huart1, (uint8_t *)buffer, strlen(buffer), 200);
}

void write_string_to_sd(char string[]){ //writes string
	char buffer[50];
	sprintf(buffer, "%s\r\n", string);
	HAL_UART_Transmit(&huart1, (uint8_t *)buffer, strlen(buffer), 100);
}

/*This function separates integer and decimal parts of float since float writing doesnt safely work with freertos.
 Thus, it breaks the float number into two integers*/ //mudar rotina
void write_current(float current){ //writes current readings
	char buffer[50];
	int int_current, dec_current; //integer and decimal parts

	int_current = (int) current;
	dec_current = (int) ((current - int_current)*100); //two decimal places
	if (dec_current < 10){
		sprintf(buffer, "current: %d.0%d\r\n", int_current, dec_current);
	}
	else{
		sprintf(buffer, "current: %d.%d\r\n", int_current, dec_current);
	}
	HAL_UART_Transmit(&huart1, (uint8_t *)buffer, strlen(buffer), 100);
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
	if(val>0 && val<100){ //duty cycle in percentage
		TIM2->CCR2 = val; //duty cycle value. Max configuration according to ARR register.
	}
}

void set_dutycycle(float avg_voltage){ //returns corresponding dutycycle in order to achieve pre-defined average voltage value
   //implement here

}
