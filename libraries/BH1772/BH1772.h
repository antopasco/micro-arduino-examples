/*
	BH1772.h - BH1772 library for iProtoXi Micro
	
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
#ifndef BH1772_h
#define BH1772_h

#define BH1772_ADDR 0x38

enum sensor_mode_control_t {STANDBY_ALP, DISABLE_ALP, TRIGGER_ALP, ON_ALP};
enum ir_current_t {IR_CURRENT_5mA, IR_CURRENT_10mA, IR_CURRENT_20mA, IR_CURRENT_50mA, IR_CURRENT_100mA, IR_CURRENT_150mA, IR_CURRENT_200mA};
enum select_trigger_t {PROXIMITY = 1, AMBIENT = 2};
enum BH1772_meas_rate_t {MEAS_RATE_10ms, MEAS_RATE_20ms, MEAS_RATE_30ms, MEAS_RATE_50ms, MEAS_RATE_70ms, MEAS_RATE_100ms, MEAS_RATE_200ms, MEAS_RATE_500ms, MEAS_RATE_1000ms, MEAS_RATE_2000ms, MAX_MEAS_RATE};

class BH1772 {
private:
	
public:
	void reset(void);
	void ambientMode(sensor_mode_control_t);
	void proximityMode(sensor_mode_control_t);
	void irCurrent(ir_current_t);
	void trigger(select_trigger_t);
	void proximityMeasRate(BH1772_meas_rate_t);
	bool ambientMeasRate(BH1772_meas_rate_t);
	uint16_t readAmbient(void);
	uint8_t readProximity(void);
	void selectInterrupt(select_trigger_t);
	void proximityInterruptTresholdHigh(uint8_t);
	void proximityInterruptTresholdLow(uint8_t);
	void ambientInterruptTresholdHigh(uint16_t);
	void ambientInterruptTresholdLow(uint16_t);
	void ambientSensitivity(uint8_t);
	bool proximityPersistence(uint8_t);
	bool ambientPersistence(uint8_t);
	uint8_t readReg(uint8_t);
	void writeReg(uint8_t, uint8_t);
	};

#endif