/*
 *   itg3200.cpp
 *
 *   Copyright (C) 2010  Ricardo Arturo Cabral <ing dot cabral dot mejia at gmail dot com>. All rights reserved.
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  See <license.txt>, if not, see 
 *   <http://www.gnu.org/licenses/>.
 */
#include "Wire.h"
//#include "WConstants.h" 
#include "itg3200.h"


ITG3200::ITG3200()
{
  Wire.begin();
};

void ITG3200::begin(int address)
{
  _gyro_address = address;
  write(GYRO_REG_PWR_MGM, 0);
  // divider = 0 => Fsample = 1kHz / (0+1) = 1kHz or 1ms per sample
  write(GYRO_REG_SMPLRT_DIV, 0x00);
  // 0b000 11  110 = 0x1E (+-2000deg/sec & LP 5Hz & OR 1kHz)
  write(GYRO_REG_DLPF_FS, 0x1E);
  write(GYRO_REG_INT_CFG, 0);
}

void ITG3200::begin(int address,byte pwr_mgm, byte fs_lpf, byte smplrt_div, byte int_cfg) {
  // Power Management
  _gyro_address = address;
  write(GYRO_REG_PWR_MGM, pwr_mgm);
  
  // Sample rate divider
  //_smplrt_div = smplrt_div;
  write(GYRO_REG_SMPLRT_DIV, smplrt_div);
  
  //Frequency select and digital low pass filter
  //_fs_lpf = fs_lpf;
  write(GYRO_REG_DLPF_FS, 0x1F & fs_lpf);
  
  //Interrupt configuration
  //_int_cfg = int_cfg;
  write(GYRO_REG_INT_CFG, 0xF5 & int_cfg);
}

int ITG3200::getX() {
  return (read(GYRO_REG_X_L) | read(GYRO_REG_X_H)<<8); 
}

int ITG3200::getY() {
  return (read(GYRO_REG_Y_L) | read(GYRO_REG_Y_H)<<8);
}

int ITG3200::getZ() {
  return (read(GYRO_REG_Z_L) | read(GYRO_REG_Z_H)<<8);
}

float ITG3200::getTemperature(){
  return (((float)((read(GYRO_REG_TEMP_L) | read(GYRO_REG_TEMP_H)<<8) + GYRO_TEMP_OFFSET))/GYRO_TEMP_SENSITIVITY) + GYRO_TEMP_OFFSET_CELSIUS;
}

void ITG3200::reset() {
  write(GYRO_REG_PWR_MGM, GYRO_RESET);
}

void ITG3200::sleep() {
  byte t = read(GYRO_REG_PWR_MGM);
  write(GYRO_REG_PWR_MGM, t | GYRO_SLEEP);
}

void ITG3200::wake(){
  byte t = read(GYRO_REG_PWR_MGM);
  write(GYRO_REG_PWR_MGM, t & ~GYRO_SLEEP);
}

void ITG3200::standBy(byte axis) {
  byte t = read(GYRO_REG_PWR_MGM);
  write(GYRO_REG_PWR_MGM, t & ~axis);
}

byte ITG3200::getAddress()
{
  return read(GYRO_REG_WHOAMI);
}

void ITG3200::setAddress(byte newAddress)
{
  write(GYRO_REG_WHOAMI, newAddress);
}

void ITG3200::setInterruptConfig(byte config)
{
  // bit 3 and 1 must be zero
  write(GYRO_REG_INT_CFG, 0xF5 & config);
}

bool ITG3200::isInterruptRawDataReady()
{
  byte result = read(GYRO_REG_INT_STS);
  return (result & GYRO_INT_DATA) == GYRO_INT_DATA;
}

bool ITG3200::isInterruptReady()
{
  byte result = read(GYRO_REG_INT_STS);
  return (result & GYRO_INT_READY) == GYRO_INT_READY;
}

byte ITG3200::getInterruptConfig()
{
  return read(GYRO_REG_INT_CFG);
}

void ITG3200::setClockSource(byte clockSource)
{
  if (clockSource >= 6) // 6 and 7 are reserved
    return;
  write(GYRO_REG_PWR_MGM, 0xF8 & clockSource);
}

void ITG3200::write(byte reg, byte val) {
  Wire.beginTransmission(_gyro_address);
  Wire.write(reg);
  Wire.write(val);
  Wire.endTransmission();
}

byte ITG3200::read(byte reg) {
  Wire.beginTransmission(_gyro_address);
  Wire.write(reg);
  Wire.endTransmission();
  Wire.beginTransmission(_gyro_address);
  Wire.requestFrom(_gyro_address,  1);
  while (!Wire.available()); // block until data is available
  byte buf = Wire.read();
  Wire.endTransmission();
  return buf;
}
