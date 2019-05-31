/*STM32 library for communicating with accelerometer sensor ADXL345.
It uses a HAL layer for implementing I2C communication.
This file must be added to the "Src" folder of the STM32 project.*/

#include "adxl345.h"
extern I2C_HandleTypeDef hi2c1; //I2C channel
extern uint8_t ADXL_OK;

#define adxl_address 0x53<<1 //0x53 addr left-shifted by 1 bit

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


void adxl_init(){ //sets initial configuration of the device
	uint8_t chipid;

	HAL_Delay(500); //wait for device to power on completely

	if (HAL_I2C_IsDeviceReady(&hi2c1, adxl_address, 3, 1000) != HAL_OK){ //tries initialization for 3 times during 1s each
		ADXL_OK = 0x01; //error code 1
		adxl_error();
	}
	else{
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
}

void adxl_write_data(uint8_t reg, uint8_t value){ //write data to specified register of peripheral device
	uint8_t data[2];
	data[0] = reg;
	data[1] = value;
	HAL_I2C_Master_Transmit(&hi2c1, adxl_address, data, 2, 100);
}

char adxl_read_reg(uint8_t reg){ //reads value from specified register
	uint8_t value;
	HAL_I2C_Mem_Read (&hi2c1, adxl_address, reg, 1, &value, 1, 100);
	return value;
}

void adxl_read_data(float *x_scaled, float *y_scaled, float *z_scaled){ //reads acceleration data (in g's)
	uint8_t data_rec[6];
	int16_t x_raw, y_raw, z_raw;

	HAL_I2C_Mem_Read(&hi2c1, adxl_address, DATAX0, 1, data_rec, 6, 100); //reads acceleration data

	//raw values
	x_raw = (data_rec[1]<<8) | data_rec[0]; //shifting of Most Significant Byte and concatenation with Least Significant Byte
	y_raw = (data_rec[3]<<8) | data_rec[2];
	z_raw = (data_rec[5]<<8) | data_rec[4];

	//scaled values
	*x_scaled = x_raw*0.0039; //multiplication by scale factor, readings in g
	*y_scaled = y_raw*0.0039;
	*z_scaled = z_raw*0.0039;

}
