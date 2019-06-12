/*STM32 library for communicating with gyroscope sensor ITG3200.
It uses a HAL layer for implementing I2C communication.
This file must be added to the "Src" folder of the STM32 project.*/

#include "itg3200.h"
#include "string.h"

extern I2C_HandleTypeDef hi2c1; // I2C channel

#define itg_address 0x68<<1 // 0x68 addr left-shifted by 1 bit 1101000 -> 11010000

// register definitions

// who am i register
#define WHO_AM_I 0x00 // WHO AM I: saves addr values of the device

// filter and sample rate configurations
#define SMPLRT_DIV 0x15 // SAMPLE RATE DIVIDER: (0 to 255)
#define DLPF_FS 0x16 	// DIGITAL LOW PASS FILTER: defines filter bandwidth and internal sample rate

// data registers
#define GYRO_XOUT_H 0x1D // msb
#define GYRO_XOUT_L 0x1E // lsb
#define GYRO_YOUT_H 0x1F
#define GYRO_YOUT_L 0x20
#define GYRO_ZOUT_H 0x21
#define GYRO_ZOUT_L 0x22

// interrupt registers
#define INT_CFG 0x17 	// interrupt configuration register
#define INT_STATUS 0x1A // interrupt check

//power management configuration
#define PWR_MGM 0x3E

uint8_t ITG_OK;
const float ITG_SENSITIVITY = 14.375; 	// from datasheet
float gyro_offsets[3]; //offsets


void itg_init_no_interrupt(void){ 		// sets initial configuration of the device

	ITG_OK = 0X00;
	HAL_Delay(500); //wait for device to power on completely

	while(HAL_I2C_IsDeviceReady(&hi2c1, itg_address, 5, 100) != HAL_OK){ //tries initialization for 5 times during 100ms each
		ITG_OK = 0x01; 						// error code 1
		itg_error();
		HAL_Delay(500); //500ms delay to try connection again
	}
   //device is connected
    itg_write_data(PWR_MGM, 0x00); 		// default settings and internal oscillator as clk
    itg_write_data(DLPF_FS, 0x1E); 		// set FS_SEL to 03h and DLPF_CFG to 6 (5hz cutoff freq. and 1kHz sampling rate)
    itg_write_data(SMPLRT_DIV, 0x00);   // divider = 0 => Fsample = 1kHz / (0+1) = 1kHz or 1ms per sample
    itg_write_data(INT_CFG, 0x00); 		// no use of interrupt
}

void itg_init_set_interrupt(void){ // sets initial configuration of the device

  ITG_OK = 0X00;
  HAL_Delay(500); //wait for device to power on completely

  if(HAL_I2C_IsDeviceReady(&hi2c1, itg_address, 5, 100) != HAL_OK){
    ITG_OK = 0x01; //error code 1
    itg_error();
  }
  else{ //device is connected
    itg_write_data(PWR_MGM, 0x00); 		// default settings and internal oscillator as clk
    itg_write_data(DLPF_FS, 0x1E); 		// set FS_SEL to 03h and DLPF_CFG to 6 (5hz cutoff freq. and 1kHz sampling rate)
    itg_write_data(SMPLRT_DIV, 0x13);  	// divider = 19 => Fsample = 1kHz / (19+1) = 50Hz --> 20ms per sample
    itg_write_data(INT_CFG, 0x21); 		// interrupt is used. latch until interrupt is cleared and enable int when data is available 00100001
  }
}

void itg_zero_fnc(){ //zeroing the device
  float gyro_data[3] = {0, 0, 0}; //sensor readings
  float sum_x = 0, sum_y = 0, sum_z = 0; // save each read value
  int i = 0;

  for(i=0;i<3;i++){
  	gyro_offsets[i] = 0;
  }

  itg_read_data_no_interrupt(gyro_data); //disconsider first reading
  HAL_Delay(20);

  for(i=0; i<100; i++){//gets 100 gyroscope samples
    itg_read_data_no_interrupt(gyro_data); //reads values with zeroed offset
    sum_x += gyro_data[0]; //accumulates readings
    sum_y += gyro_data[1];
    sum_z += gyro_data[2];
    HAL_Delay(20); //20ms in-between samples
  }

  gyro_offsets[0] = sum_x/100; //average value
  gyro_offsets[1] = sum_y/100;
  gyro_offsets[2] = sum_z/100;

}

void itg_write_data(uint8_t reg, uint8_t value){ // write to register to set configurations
  uint8_t data[2];
	data[0] = reg;
	data[1] = value;
  HAL_I2C_Master_Transmit(&hi2c1, itg_address, data, 2, 100);
}

char itg_read_reg(uint8_t reg){
  uint8_t value;
  HAL_I2C_Mem_Read(&hi2c1, itg_address, reg, 1, &value, 1, 100);
  return value;
}

void itg_read_data_no_interrupt(float *gyro_data){  // reads gyro data in stardard mode
  uint8_t data_rec[6]; // vector to read gyro values (x, y and z)
  int16_t x_raw, y_raw, z_raw;

  HAL_I2C_Mem_Read(&hi2c1, itg_address, (GYRO_XOUT_H), 1, data_rec, 6, 100); //read 6 bytes (from GYRO_XOUT_H to GYRO_ZOUT_L)

    x_raw = (data_rec[0]<<8) | data_rec[1]; // shifting of Most Significant Byte and concatenation with Least Significant Byte
	y_raw = (data_rec[2]<<8) | data_rec[3];
	z_raw = (data_rec[4]<<8) | data_rec[5];

	//scaled values

	gyro_data[0] = x_raw/ITG_SENSITIVITY - gyro_offsets[0]; // division by sensitivity. readings in deg/s
	gyro_data[1] = y_raw/ITG_SENSITIVITY - gyro_offsets[1];
	gyro_data[2] = z_raw/ITG_SENSITIVITY - gyro_offsets[2];

}

void itg_read_data_when_ready(float *gyro_data){  // reads gyro data in interrupt mode (reads when new data is available)
  uint8_t data_rec[6]; // vector to read gyro values (x, y and z)
  int16_t x_raw, y_raw, z_raw;
  uint8_t data_read = 0x00;

  while(data_read != 0x01){
    if(itg_read_reg(INT_STATUS) == 0x01){ // new data is ready
      HAL_I2C_Mem_Read(&hi2c1, itg_address, (GYRO_XOUT_H), 1, data_rec, 6, 100); // read 6 bytes (from GYRO_XOUT_H to GYRO_ZOUT_L)
      data_read = 0x01;
    }
  }

  x_raw = (data_rec[0]<<8) | data_rec[1]; // shifting of Most Significant Byte and concatenation with Least Significant Byte
  y_raw = (data_rec[2]<<8) | data_rec[3];
  z_raw = (data_rec[4]<<8) | data_rec[5];

  //scaled values
	gyro_data[0] = x_raw/ITG_SENSITIVITY - gyro_offsets[0]; // division by sensitivity. readings in deg/s
	gyro_data[1] = y_raw/ITG_SENSITIVITY - gyro_offsets[1];
	gyro_data[2] = z_raw/ITG_SENSITIVITY - gyro_offsets[2];
}

void itg_error(){
	//error
	if(ITG_OK == 0x01){ //couldnt initialize device
	}
}
