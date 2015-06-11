/*
	LPS331.cpp - LPS331 library for iProtoXi Micro
	
	Free software under the MIT License (MIT)

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

#include <LPS331.h>
#include <Wire.h>
#include <math.h>
#include <stdint.h>
#include <arduino.h>

void LPS331::init(void)
{
	writeReg(0x20, 0x84);
}
void LPS331::reset(void)
{
	writeReg(0x21, 0x80);
}
void LPS331::oneShotMeasure(void)
{
	writeReg(0x21, 0x01);
}
void LPS331::autoZero(void)
{
	writeReg(0x21, 0x02);
}
void LPS331::measureRate(LPS331_meas_rate_t measureRate)
{
	uint8_t val20 = readReg(0x20);
	writeReg(0x20, (val20 & 0x7F));
	switch(measureRate)
	{
	case ONE_SHOT:
		val20 = (0x8F & val20) | 0x00;
		break;
	case MEAS_RATE_1Hz:
	    val20 = (0x8F & val20) | 0x10;
		break;
	case MEAS_RATE_7Hz:
	    val20 = (0x8F & val20) | 0x50;
		break;
	case MEAS_RATE_12Hz5:
	    val20 = (0x8F & val20) | 0x60;
		break;
	case MEAS_RATE_25Hz:
	    val20 = (0x8F & val20) | 0x70;
		break;
	}
	writeReg(0x20, val20);
}
void LPS331::pressureDelta(bool enable)
{
	uint8_t val20 = readReg(0x20);
	if(enable)
		val20 = (0xFD & val20);
	else
		val20 = val20 | 0x02;	
	writeReg(0x20, val20);
}
int32_t LPS331::readPresRaw(void)
{
	uint8_t extraLowByte = readReg(0x28);
	uint8_t lowByte = readReg(0x29);
	int8_t highByte = readReg(0x2A);
	int32_t HB = highByte;
	uint32_t LB = lowByte;
	uint32_t XLB = extraLowByte;
	return (HB << 16) + (LB << 8) + XLB;
}
int16_t LPS331::readTempRaw(void)
{
	uint8_t lowByte = readReg(0x2B);
	uint8_t highByte = readReg(0x2C);
	return (highByte << 8) + lowByte;
}
float LPS331::mBarPress(int32_t pressure)
{
	float presFloat = pressure;
	return presFloat/4096;
}
float LPS331::celsiusTemp(int16_t temperature)
{
	float tempFloat = temperature;
	return (tempFloat/480)+42.5;
}
uint8_t LPS331::readReg(uint8_t address)
{
	uint8_t reg;
	Wire.beginTransmission(LPS331_ADDR);
    Wire.write(address);
    Wire.endTransmission();
    Wire.requestFrom(LPS331_ADDR, 1);
	reg = Wire.read();
	Wire.endTransmission();
	return reg;
}
void LPS331::writeReg(uint8_t address, uint8_t value)
{
	Wire.beginTransmission(LPS331_ADDR);
	Wire.write(address);
	Wire.write(value);
	Wire.endTransmission(true);
}