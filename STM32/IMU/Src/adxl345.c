/*STM32 library for communicating with accelerometer sensor ADXL345.
It uses a HAL layer for implementing I2C communication.
This file must be added to the "Src" folder of the STM32 project.*/

#include "adxl345.h"
#include "string.h"

extern I2C_HandleTypeDef hi2c1; //I2C channel

#define adxl_address 0x53<<1 //0x53 addr left-shifted by 1 bit
//#define adxl_address 0x1D<<1 //0x53 addr left-shifted by 1 bit

//register definitions
#define DEVID 0x00 //device id

//data registers
#define DATAX0 0x32
#define DATAX1 0x33
#define DATAY0 0x34
#define DATAY1 0x35
#define DATAZ0 0x36
#define DATAZ1 0x37

//offset definitions
#define OFSX 0x1E
#define OFSY 0x1F
#define OFSZ 0x20

//power control
#define POWER_CTL 0x2D

//data format
#define DATA_FORMAT 0x31


uint8_t ADXL_OK;
const float ACC_SCALE_CONST = 0.0039;

float acc_offsets[3]; //offsets vector

void adxl_init(){ //sets initial configuration of the device
	uint8_t chipid;

	ADXL_OK = 0X00;
	HAL_Delay(500); //wait for device to power on completely

	while(HAL_I2C_IsDeviceReady(&hi2c1, adxl_address, 1, 10) != HAL_OK){ //tries initialization for 5 times during 100ms each
		ADXL_OK = 0x01; //error code 1
		adxl_error();
		HAL_Delay(500); //500ms delay to try connection again
	}
	//device is connected
	chipid = adxl_read_reg(DEVID);  //reads identifier addr which must be 0xE5
	if (chipid != 0xE5){ //device not working properly
		ADXL_OK = 0x02; //error code 2
		adxl_error();
	}
	else{
		//initial device configuration
		adxl_write_data(POWER_CTL, 0); //reset all POWER_CTL bits
		adxl_write_data(POWER_CTL, 0x08); //writes 1000 (measure-bit set to 1)
		adxl_write_data(DATA_FORMAT, 0X00); // +- 2g range
	}
}

void adxl_zero_fnc(){ //zeroing the device
  float acc_data[3] = {0, 0, 0}; //vector initialization
  float sum_x = 0, sum_y = 0, sum_z = 0; // save each read value
  int i = 0;

  for(i=0;i<3;i++){
	acc_offsets[i] = 0;
  }

  adxl_read_data(acc_data); //disconsider first reading
  HAL_Delay(20);

  for(i=0; i<100; i++){//gets 100 gyroscope samples
    adxl_read_data(acc_data);
    sum_x += acc_data[0]; //accumulates readings
    sum_y += acc_data[1];
    sum_z += acc_data[2];
    HAL_Delay(20); //20ms in-between samples
  }

  acc_offsets[0] = sum_x/100; //average value
  acc_offsets[1] = sum_y/100;
  acc_offsets[2] = sum_z/100 - 1;

}

void adxl_write_data(uint8_t reg, uint8_t value){ //write data to specified register of peripheral device
	uint8_t data[2];
	data[0] = reg;
	data[1] = value;
	HAL_I2C_Master_Transmit(&hi2c1, adxl_address, data, 2, 100);
}

char adxl_read_reg(uint8_t reg){ //reads value from specified register
	uint8_t value;
	HAL_I2C_Mem_Read(&hi2c1, adxl_address, reg, 1, &value, 1, 100);
	return value;
}

void adxl_read_data(float *acc_data){ //reads acceleration data (in g's)
	uint8_t data_rec[6];
	int16_t x_raw, y_raw, z_raw;

	HAL_I2C_Mem_Read(&hi2c1, adxl_address, DATAX0, 1, data_rec, 6, 100); //reads acceleration data

	//raw values
	x_raw = (data_rec[1]<<8) | data_rec[0]; //shifting of Most Significant Byte and concatenation with Least Significant Byte
	y_raw = (data_rec[3]<<8) | data_rec[2];
	z_raw = (data_rec[5]<<8) | data_rec[4];

	//scaled values
	acc_data[0] = x_raw*ACC_SCALE_CONST - acc_offsets[0]; //multiplication by scale factor, readings in g
	acc_data[1] = y_raw*ACC_SCALE_CONST - acc_offsets[1];
	acc_data[2] = z_raw*ACC_SCALE_CONST - acc_offsets[2];

}

void adxl_error(){
	//error
	if(ADXL_OK == 0x01){ //couldnt initialize device
		//Accelerometer could not be initialized!
	}
	else if (ADXL_OK == 0x02){
		//Invalid accelerometer ID!
	}
}

