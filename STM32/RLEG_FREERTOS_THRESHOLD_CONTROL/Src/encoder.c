/*STM32 library for communicating with AMT20 encoder.
It uses a HAL layer for implementing SPI communication.
This file must be added to the "Src" folder of the STM32 project.*/
#include "encoder.h"
extern SPI_HandleTypeDef hspi2;

//command and response definitions
#define NOP_A5 0x00
#define NOP_A5_RESPONSE 0xA5
#define READ_POS 0x10
#define SET_ZERO_POS 0x70
#define SET_ZERO_POS_SUCCESS 0x80

#define WAIT_RESPONSE_DELAY 5

char encoder_transmit_command(uint8_t command){ //transmit a command to execute action
  uint8_t command_rec;

  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET); // pull the cs pin low
	HAL_SPI_TransmitReceive(&hspi2, &command, &command_rec, 2, 1); //transmit and receive data
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET); // pull the cs pin high

  return command_rec;
}

char encoder_get_data(uint8_t command){ //transmit a command to read data back
  uint8_t data;

  HAL_GPIO_WritePin (GPIOB, GPIO_PIN_12, GPIO_PIN_RESET); // pull the cs pin low
	HAL_SPI_TransmitReceive(&hspi2, &command, &data, 2, 1); //transmit and receive data
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET); // pull the cs pin high

  return data;
}

void encoder_init(){
	int i=0;

	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET); // pull the cs pin high

	HAL_Delay(100); // wait 100ms for encoder initialization

	 for(i=0; i<3; i++) //clean device buffer
	 {
	 	encoder_transmit_command(NOP_A5);
	 	HAL_Delay(250);
	 }

}

int encoder_set_zero_pos(){ //zeroes sensor
  int i=0;
  uint8_t command_rec;

	command_rec = encoder_transmit_command(SET_ZERO_POS); //send set zero pos command and read answer
	HAL_Delay(20);

  while(command_rec!= SET_ZERO_POS_SUCCESS && i<10){ //tries to get a sucess answer for 10 times max
    command_rec = encoder_transmit_command(NOP_A5); // keep sending nop commands and read answer
    HAL_Delay(20);
    i++;
  }

	if (command_rec == SET_ZERO_POS_SUCCESS){ //checks if got success response
		write_string_to_sd("Encoder was zeroed!");
		return 1; //zeroing sucessful
	}
	else{
		write_string_to_sd("Encoder could not be zeroed!");
		return 0; //zeroing unsucessful
	}
	encoder_init(); //wait and clear buffer after zeroing device
}

void encoder_read_pos(float *position){ //reads encoder position
  uint8_t command_rec, buffer;
  uint16_t data;

  command_rec = encoder_transmit_command(READ_POS); // sends read position command and tries to read an echo
  HAL_Delay(WAIT_RESPONSE_DELAY); //delay to wait sensor response
  command_rec = encoder_transmit_command(NOP_A5);
  HAL_Delay(WAIT_RESPONSE_DELAY);
  while(command_rec != READ_POS){  // echo of the command
	  command_rec = encoder_transmit_command(NOP_A5); // keep sending nop commands and read answer
     HAL_Delay(WAIT_RESPONSE_DELAY);
  }
  buffer = encoder_get_data(NOP_A5); // send first nop command after received the 0x10 echo and get msb to data_rec
  data = (buffer<<8) | 0x00; // shitf msb
  HAL_Delay(WAIT_RESPONSE_DELAY);
  buffer = encoder_get_data(NOP_A5); // send second nop command after received the 0x10 echo and get lsb to data_rec
  data = data | buffer;
  HAL_Delay(WAIT_RESPONSE_DELAY); // delay in-between reads

  //conversion of read data into position in degrees. could be put in a separate function if needed
  *position = 360 - ((data)*360)/4096.0; //data conversion to angle in the possible direction of movement

  if (*position==360){ //0 position from zeroing
    *position = 0;
  }

}
