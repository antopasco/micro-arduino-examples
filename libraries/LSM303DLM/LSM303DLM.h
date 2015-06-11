/*
	LSM303DLM.h - LSM303DLM library for iProtoXi Micro
	
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
#include <stdint.h>
#ifndef LSM303DLM_h
#define LSM303DLM_h

#define LSM303_ACC_ADDR			0x18

#define LSM303_CTRL_REG1_A   	0x20 //Accelerometer Control Register 1 register address
#define LSM303_CTRL_REG2_A 		0x21
#define LSM303_CTRL_REG3_A 		0x22
#define LSM303_CTRL_REG4_A 		0x23
#define LSM303_CTRL_REG5_A		0x24

#define LSM303_CR1A_DEFAULT		0x37 //400Hz, x, y & z axes enabled
#define LSM303_CR2A_DEFAULT		0x00
#define LSM303_CR3A_DEFAULT		0x00
#define LSM303_CR4A_DEFAULT		0x80
#define LSM303_CR5A_DEFAULT		0x00

enum ODR_t {
	ODR_50HZ 	= 0x20,
	ODR_100HZ 	= 0x28,
	ODR_400HZ 	= 0x30,
	ODR_1000HZ 	= 0x38,
	ODR_0HZ5 	= 0x40,
	ODR_1HZ 	= 0x60,
	ODR_2HZ 	= 0x80,
	ODR_5HZ 	= 0xA0,
	ODR_10HZ 	= 0xC0 };
enum scale_t {
	SCALE_2g	= 0x80,
	SCALE_4g	= 0x90,
	SCALE_8g	= 0xB0 };
	
	
class LSM303 {
	uint16_t scale;
public:
	bool setODR(ODR_t); //set the desired output data rate
	bool readRawXYZ(int16_t&, int16_t&, int16_t&); //read the raw sensor values
	void mgScale(int16_t&, int16_t&, int16_t&); //transform values into millig
	uint16_t getResultant(int16_t, int16_t, int16_t vector3 = 0); //calculate resultant vector
	void writeReg(uint8_t, uint8_t); //writes the value into accelerometers register
	uint8_t readReg(uint8_t); //reads the value from accelerometers register
	void init(void); //default basic config for Accelerometer (ODR 400Hz, all axes enabled, output registers not updated between MSB and LSB reading)
	bool setScale(scale_t); //set accelerometer maximum scale
	int8_t getScale(void); //get accelerometer current scale setting
	};
	
#endif