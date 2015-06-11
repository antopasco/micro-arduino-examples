/*
	LP5523.cpp - LP5523 library for iProtoXi Micro
	
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

#include <BH1772.h>
#include <Wire.h>
#include <math.h>
#include <stdint.h>
#include <arduino.h>

void BH1772::reset(void)
{
	writeReg(0x40, 0x04);
}
void BH1772::ambientMode(sensor_mode_control_t ALS_mode)
{
	uint8_t reg40 = readReg(0x40);
	reg40 = (reg40 & 0xFC) | ALS_mode;
	writeReg(0x40, reg40);
}
void BH1772::proximityMode(sensor_mode_control_t PS_mode)
{
	uint8_t reg41 = readReg(0x41);
	reg41 = (reg41 & 0xFC) | PS_mode;
	writeReg(0x41, reg41);
}
void BH1772::irCurrent(ir_current_t current)
{
	writeReg(0x42, (0x18 + current));
}
void BH1772::trigger(select_trigger_t trigger)
{
	writeReg(0x44, trigger);
}
void BH1772::proximityMeasRate(BH1772_meas_rate_t meas_rate)
{
	uint8_t reg45 = meas_rate;
	if(reg45 == MAX_MEAS_RATE)
		reg45 = 0x00;
	writeReg(0x45, reg45);
}
bool BH1772::ambientMeasRate(BH1772_meas_rate_t meas_rate)
{
	uint8_t reg46;
	switch(meas_rate)
	{
	case MAX_MEAS_RATE:
		reg46 = 0x80;
		break;
	case MEAS_RATE_100ms:
		reg46 = 0x00;
		break;
	case MEAS_RATE_200ms:
		reg46 = 0x01;
		break;
	case MEAS_RATE_500ms:
		reg46 = 0x02;
		break;
	case MEAS_RATE_1000ms:
		reg46 = 0x03;
		break;
	case MEAS_RATE_2000ms:
		reg46 = 0x04;
		break;
	default:
		return 0;
		break;
	}
	writeReg(0x46, reg46);
	return 1;
}
uint16_t BH1772::readAmbient(void)
{
	return readReg(0x4C) + readReg(0x4D)*256;
}
uint8_t BH1772::readProximity(void)
{
	return readReg(0x4F);
}
void BH1772::selectInterrupt(select_trigger_t trigger)
{
	writeReg(0x52, (0x08 + trigger));
}
void BH1772::proximityInterruptTresholdHigh(uint8_t treshold)
{
	writeReg(0x53, treshold);
}
void BH1772::proximityInterruptTresholdLow(uint8_t treshold)
{
	writeReg(0x53, treshold);
}
void BH1772::ambientInterruptTresholdHigh(uint16_t treshold)
{
	writeReg(0x56, treshold%256);
	writeReg(0x57, treshold/256);
}
void BH1772::ambientInterruptTresholdLow(uint16_t treshold)
{
	writeReg(0x58, treshold%256);
	writeReg(0x59, treshold/256);
}
void BH1772::ambientSensitivity(uint8_t sensitivity)
{
	uint8_t val40 = readReg(0x40);
	ambientMode(STANDBY_ALP);
	writeReg(0x5A, sensitivity);
	writeReg(0x40, val40);
}
bool BH1772::proximityPersistence(uint8_t persistence)
{
	uint8_t val5B;
	if(persistence > 15)
		return 0;
	else
	{
		val5B = readReg(0x5B);
		val5B = (val5B & 0xF0) | persistence;
		writeReg(0x5B, val5B);
		return 1;
	}
}
bool BH1772::ambientPersistence(uint8_t persistence)
{
	uint8_t val5B;
	if(persistence > 15)
		return 0;
	else
	{
		val5B = readReg(0x5B);
		val5B = (val5B & 0x0F) | (persistence << 4);
		writeReg(0x5B, val5B);
		return 1;
	}
}
uint8_t BH1772::readReg(uint8_t address)
{
	uint8_t reg;
	Wire.beginTransmission(BH1772_ADDR);
    Wire.write(address);
    Wire.endTransmission();
    Wire.requestFrom(BH1772_ADDR, 1);
	reg = Wire.read();
	Wire.endTransmission();
	return reg;
}
void BH1772::writeReg(uint8_t address, uint8_t value)
{
	Wire.beginTransmission(BH1772_ADDR);
	Wire.write(address);
	Wire.write(value);
	Wire.endTransmission(true);
}