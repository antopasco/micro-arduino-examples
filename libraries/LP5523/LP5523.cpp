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

#include <LP5523.h>
#include <Wire.h>
#include <math.h>
#include <stdint.h>
#include <arduino.h>

uint8_t genCodePointer = 0;

bool LP5523::genCodePointerIncrement(void)
{
	if(genCodePointer<0x60) //check we are not outside program memory bounds 
	{
		genCodePointer++;
		return 1;
	}
	else //return 0 if not possible to write anything more to program memory
		return 0;
}
bool LP5523::op_ledMapping(uint16_t leds) //led mapping for opcode
{
	genCode[genCodePointer] = leds;
	return genCodePointerIncrement();
}
bool LP5523::op_ramp(step_time_t step_time, sign_t sign, uint8_t increments) //ramp function for op code
{
	genCode[genCodePointer] = (step_time << 9) + (sign << 8) + increments; 
	return genCodePointerIncrement();
}
bool LP5523::op_ramp(uint8_t step_time, sign_t sign, uint8_t increments, prescale_t prescale) //raw version of ramp function for op code
{
	genCode[genCodePointer] = (prescale << 14) + (step_time << 9) + (sign << 8) + increments;
	return genCodePointerIncrement();
}
bool LP5523::op_set_pwm(uint8_t value) //set pwm function for opcode
{
	genCode[genCodePointer] = 0x4000 + value;
	return genCodePointerIncrement();
}
bool LP5523::op_wait(step_time_t step_time) //wait function for opcode
{
	genCode[genCodePointer] = (step_time << 9);
	return genCodePointerIncrement();
}
bool LP5523::op_wait(uint8_t step_time, prescale_t prescale) //wait function for opcode raw version
{
	genCode[genCodePointer] = (prescale << 14) + (step_time << 9);
	return genCodePointerIncrement();
}
bool LP5523::op_mux_ld_start(uint8_t SRAM_address) //mux load start function for opcode
{
	genCode[genCodePointer] = 0x9E00 + SRAM_address; //address is exact program memory address
	return genCodePointerIncrement();
}
bool LP5523::op_mux_map_start(uint8_t SRAM_address) //mux map start function for opcode
{
	genCode[genCodePointer] = 0x9C00 + SRAM_address; //address is exact program memory address
	return genCodePointerIncrement();
}
bool LP5523::op_mux_ld_end(uint8_t SRAM_address) //mux load end function for opcode
{
	genCode[genCodePointer] = 0x9C80 + SRAM_address; //address is exact program memory address
	return genCodePointerIncrement();
}
bool LP5523::op_mux_sel(led_selection_t LED_select) //one led select function for opcode
{
	uint8_t selectedLed;
	switch(LED_select)
	{
		case LED1_R:
		selectedLed = 7;
		break;
		case LED1_G:
		selectedLed = 1;
		break;
		case LED1_B:
		selectedLed = 2;
		break;
		case LED2_R:
		selectedLed = 8;
		break;
		case LED2_G:
		selectedLed = 3;
		break;
		case LED2_B:
		selectedLed = 4;
		break;
		case LED3_R:
		selectedLed = 9;
		break;
		case LED3_G:
		selectedLed = 5;
		break;
		case LED3_B:
		selectedLed = 6;
		break;
		case GPO:
		selectedLed = 16;
		break;
	}
	genCode[genCodePointer] = 0x9D00 + selectedLed; //LED_select selects one
	return genCodePointerIncrement();
}
bool LP5523::op_mux_sel(uint8_t LED_select) //one led select function for opcode
{
	genCode[genCodePointer] = 0x9D00 + LED_select;
	return genCodePointerIncrement();
}
bool LP5523::op_mux_clr(void)
{
	genCode[genCodePointer] = 0x9D00;
	return genCodePointerIncrement();
}
bool LP5523::op_mux_map_next(void)
{
	genCode[genCodePointer] = 0x9D80;
	return genCodePointerIncrement();
}
bool LP5523::op_mux_map_prev(void)
{
	genCode[genCodePointer] = 0x9DC0;
	return genCodePointerIncrement();
}
bool LP5523::op_mux_ld_next(void)
{
	genCode[genCodePointer] = 0x9D81;
	return genCodePointerIncrement();
}
bool LP5523::op_mux_ld_prev(void)
{
	genCode[genCodePointer] = 0x9DC1;
	return genCodePointerIncrement();
}
bool LP5523::op_mux_ld_addr(uint8_t SRAM_address)
{
	genCode[genCodePointer] = 0x9F00 + SRAM_address;
	return genCodePointerIncrement();
}
bool LP5523::op_mux_map_addr(uint8_t SRAM_address)
{
	genCode[genCodePointer] = 0x9F80 + SRAM_address;
	return genCodePointerIncrement();
}
bool LP5523::op_rst(void)
{
	genCode[genCodePointer] = 0;
	return genCodePointerIncrement();
}
bool LP5523::op_branch(uint8_t loop_count, uint8_t program_address)
{
	genCode[genCodePointer] = 0xA000 + (loop_count << 7) + program_address;
	return genCodePointerIncrement();
}
bool LP5523::op_int(void)
{
	genCode[genCodePointer] = 0xC400;
	return genCodePointerIncrement();
}
bool LP5523::op_end(bool op_int, bool op_rst)
{
	genCode[genCodePointer] = 0xC000 + (op_int << 12) + (op_rst << 11);
	return genCodePointerIncrement();
}
bool LP5523::op_variable_load(variable_t target_variable, uint8_t value)
{
	genCode[genCodePointer] = 0x9000 + (target_variable << 10) + value;
	return genCodePointerIncrement();
}
bool LP5523::op_variable_add(variable_t target_variable, uint8_t value)
{
	genCode[genCodePointer] = 0x9100 + (target_variable << 10) + value;
	return genCodePointerIncrement();
}
bool LP5523::op_variable_add(variable_t target_variable, variable_t variable1, variable_t variable2)
{
	genCode[genCodePointer] = 0x9300 + (target_variable << 10) + (variable1 << 2) + variable2;
	return genCodePointerIncrement();
}
bool LP5523::op_variable_sub(variable_t target_variable, uint8_t value)
{
	genCode[genCodePointer] = 0x9200 + (target_variable << 10) + value;
	return genCodePointerIncrement();
}
bool LP5523::op_variable_sub(variable_t target_variable, variable_t variable1, variable_t variable2)
{
	genCode[genCodePointer] = 0x9310 + (target_variable << 10) + (variable1 << 2) + variable2;
	return genCodePointerIncrement();
}
bool LP5523::op_trigger(trigger_t trigger)
{
	genCode[genCodePointer] = 0xE000 + trigger;
	return genCodePointerIncrement();
}
bool LP5523::op_jump_if_not_equal(uint8_t steps, variable_t variable1, variable_t variable2)
{
	genCode[genCodePointer] = 0x8800 + (steps << 4) + (variable1 << 2) + variable2;
	return genCodePointerIncrement();
}
bool LP5523::op_jump_if_less(uint8_t steps, variable_t variable1, variable_t variable2)
{
	genCode[genCodePointer] = 0x8A00 + (steps << 4) + (variable1 << 2) + variable2;
	return genCodePointerIncrement();
}
bool LP5523::op_jump_if_greater(uint8_t steps, variable_t variable1, variable_t variable2)
{
	genCode[genCodePointer] = 0x8C00 + (steps << 4) + (variable1 << 2) + variable2;
	return genCodePointerIncrement();
}
bool LP5523::op_jump_if_equal(uint8_t steps, variable_t variable1, variable_t variable2)
{
	genCode[genCodePointer] = 0x8E00 + (steps << 4) + (variable1 << 2) + variable2;
	return genCodePointerIncrement();
}
void LP5523::resetCode(void)
{
	for(uint8_t k=0 ; k<96 ; k++)
	genCode[k] = 0;
	genCodePointer = 0;
}
void LP5523::uploadCode(void)
{
	for(uint8_t i=0 ; i<6 ; i++ )
	{
		while(readReg(0x3A) & 0x10)
		delay(1);	
		Wire.beginTransmission(LP5523_ADDR);
		Wire.write(0x4F);
		Wire.write(i);
		for(uint8_t k=i*0x10  ; k<i*0x10+0x10 ; k++)
		{
			if(k%16 == 15)
			{
				Wire.endTransmission();
				Wire.beginTransmission(LP5523_ADDR);
				Wire.write(0x6E);
			}
			Wire.write(genCode[k]/256);
			Wire.write(genCode[k]%256);
		}
		Wire.endTransmission();
	}
}
void LP5523::engineStartAddress(uint8_t engine, uint8_t startAddress)
{
	Wire.beginTransmission(LP5523_ADDR);
	Wire.write(0x4C+(engine>>1));
	Wire.write(startAddress);
	Wire.endTransmission();
}
void LP5523::engineControl(uint8_t engine, eng_instr_t instruction)
{
	uint8_t reg00 = readReg(0x00); 
	uint8_t reg01 = readReg(0x01);
	if(engine & 0x01)
	{
		reg00 = (0xCF & reg00) | ((0x03 & instruction) << 4);
		reg01 = (0xCF & reg01) | ((0x0C & instruction) << 2);
	}
	if(engine & 0x02)
	{
		reg00 = (0xF3 & reg00) | ((0x03 & instruction) << 2);
		reg01 = (0xF3 & reg01) | ((0x0C & instruction) << 0);
	}
	if(engine & 0x04)
	{
		reg00 = (0xFC & reg00) | ((0x03 & instruction) << 0);
		reg01 = (0xFC & reg01) | ((0x0C & instruction) >> 2);
	}
	writeReg(0x01, reg01);
	writeReg(0x00, reg00);
}
void LP5523::init(void)
{
	writeReg(0x00, 0x40);
	writeReg(0x36, 0x5B);
}
void LP5523::reset(void)
{
	writeReg(0x3D, 0xFF);
}
void LP5523::setCurrent(uint8_t current)
{
	Wire.beginTransmission(LP5523_ADDR);
    Wire.write(0x26);
	for(int i=0 ; i<9 ; i++)
		Wire.write(current);
    Wire.endTransmission();
}
void LP5523::setLogDimm(bool enable)
{
	uint8_t values[9];
	Wire.beginTransmission(LP5523_ADDR);
    Wire.write(0x06);
	Wire.endTransmission();
	Wire.requestFrom(LP5523_ADDR, 9);
	for(int i=0 ; i<9 ; i++)
		values[i] = Wire.read();
    Wire.endTransmission();
	Wire.beginTransmission(LP5523_ADDR);
	Wire.write(0x06);
	for(int i=0 ; i<9 ; i++)
		Wire.write((values[i] && 0xDF) | 0x20*enable );
    Wire.endTransmission();
}
void LP5523::setLed(uint8_t led, uint8_t red, uint8_t green, uint8_t blue)
{
	uint8_t regs[][3] = {
	{0x1c, 0x16, 0x17},
	{0x1d, 0x18, 0x19},
	{0x1e, 0x1a, 0x1b}
	};
	writeReg(regs[led][0], red);
	writeReg(regs[led][1], green);
	writeReg(regs[led][2], blue);
}
void LP5523::tempControl(temperature_control_t control)
{
	uint8_t value = control;
	writeReg(0x3E, value);
}
int8_t LP5523::readTemperature(void)
{
	int8_t temperature;
	if(readReg(0x3E) & 0x01)
		temperature = readReg(0x40);
	else
		temperature = readReg(0x3F);
		return temperature;
}
void LP5523::writeTemperature(uint8_t temperature)
{
	writeReg(0x40, temperature);
}
uint8_t LP5523::readReg(uint8_t address)
{
	uint8_t value;
	Wire.beginTransmission(LP5523_ADDR);
    Wire.write(address);
    Wire.endTransmission();
    Wire.requestFrom(LP5523_ADDR, 1);
	value = Wire.read();
	Wire.endTransmission();
	return value;
}
void LP5523::writeReg(uint8_t address, uint8_t value)
{
	Wire.beginTransmission(LP5523_ADDR);
	Wire.write(address);
	Wire.write(value);
	Wire.endTransmission(true);
}