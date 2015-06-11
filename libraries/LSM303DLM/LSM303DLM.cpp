/*
	LSM303DLM.cpp - LSM303DLM library for iProtoXi Micro
	
	This program is free software under the MIT License (MIT)

	Copyright 2015 iProtoXi Oy

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in all
	copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
	SOFTWARE.
*/

#include <LSM303DLM.h>
#include <Wire.h>
#include <math.h>
#include <stdint.h>
#include <arduino.h>



void LSM303::writeReg(uint8_t reg, uint8_t value)
{
	Wire.beginTransmission(LSM303_ACC_ADDR); //begin transmission to accelerometer
	Wire.write(&reg, sizeof(reg)); //send register address
	Wire.write(&value, sizeof(value)); //send register value
	Wire.endTransmission();
}

bool LSM303::setODR(ODR_t datarate)
{
	uint8_t value = readReg(LSM303_CTRL_REG1_A);
	if((value & 0xF8) != datarate) //check if there is need to change anything
	{
		value = (value & 0x07)|datarate; //set bits value(7:3) to datarate(7:3) & leave value(2:0) untouched 
		writeReg(LSM303_CTRL_REG1_A, value); //write modified value
		return true; //return true for modified value
	}
	else
		return false; //return false for no modification needed
	
}
bool LSM303::setScale(scale_t newScale)
{
	uint8_t value = readReg(LSM303_CTRL_REG4_A);
	if((value & 0x30) != newScale) //check if there is need to change anything
	{
		scale = newScale;
		scale = (((scale & 0x30) >> 4) + 1) * 2; //save the new scale
		value = (value & 0xCF)|newScale; //set bits value(5:4) to datarate(5:4) & leave value(7:6) & value(3:0) untouched 
		writeReg(LSM303_CTRL_REG4_A, value); //write modified value
		return true; //return true for modified value
	}
	else
		return false; //return false for no modification needed
}

int8_t LSM303::getScale(void) //get the scale currently in use
{
	uint8_t value;
	value = readReg(LSM303_CTRL_REG4_A);
	if(bitRead(value, 5))
		scale = 8;
	else if(bitRead(value, 4))
		scale = 4;
	else
		scale = 2;
		return scale;
}
uint8_t LSM303::readReg(uint8_t reg)
{
	uint8_t value;
	Wire.beginTransmission(LSM303_ACC_ADDR); //begin transmission to accelerometer
	Wire.write(&reg, sizeof(reg)); //send register address
	Wire.endTransmission();
	Wire.requestFrom(LSM303_ACC_ADDR, 1); //request register value 
	value = Wire.read(); //read register value
	Wire.endTransmission();
	return value; //return register value
}

bool LSM303::readRawXYZ(int16_t &x, int16_t &y, int16_t &z) //read x, y, z data
{
	int xAxis, yAxis, zAxis;
	
	Wire.beginTransmission(LSM303_ACC_ADDR); //begin transmission to accelerometer
	Wire.write(0xA8); //request all sensor data
	
	if(Wire.requestFrom(LSM303_ACC_ADDR, 6)<6) //return false if can't get data
		return false; 
	else
	{
		xAxis = Wire.read(); //read first byte
		xAxis = 256*Wire.read() + xAxis; //read second byte, multiply it by 2^8 and sum it with previous byte
		yAxis = Wire.read();
		yAxis = 256*Wire.read() + yAxis;
		zAxis = Wire.read();
		zAxis = 256*Wire.read() + zAxis;
		
		Wire.endTransmission(); //reset wire connection
		
		x = xAxis; //return values
		y = yAxis;
		z = zAxis;
		return true;
	}
}

void LSM303::mgScale(int16_t &x, int16_t &y, int16_t &z)
{
	int32_t xAxis = x, yAxis = y, zAxis = z;
	x = xAxis*1000/(1024*32/scale); //transform data to millig, for 2g scale xAxis*1000/(1024*16),
	y = yAxis*1000/(1024*32/scale); //for 4g scale xAxis*1000/(1024*8),
	z = zAxis*1000/(1024*32/scale); //for 8g scale xAxis*1000/(1024*4)
}

uint16_t LSM303::getResultant(int16_t vector1, int16_t vector2, int16_t vector3)
{
	//calculates resultant vector from 2 or 3 vectors
	uint32_t vector1_32bit = vector1, vector2_32bit = vector2, vector3_32bit = vector3;
	return sqrt(vector1_32bit*vector1_32bit+vector2_32bit*vector2_32bit+vector3_32bit*vector3_32bit); //total acceleration 
}

void LSM303::init(void)
{
	writeReg (LSM303_CTRL_REG1_A, LSM303_CR1A_DEFAULT);
	writeReg (LSM303_CTRL_REG2_A, LSM303_CR2A_DEFAULT);
	writeReg (LSM303_CTRL_REG3_A, LSM303_CR3A_DEFAULT);
	writeReg (LSM303_CTRL_REG4_A, LSM303_CR4A_DEFAULT);
	writeReg (LSM303_CTRL_REG5_A, LSM303_CR5A_DEFAULT);
	getScale();
}