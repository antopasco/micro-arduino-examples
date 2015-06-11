/*
	LP5523.h - LP5523 library for iProtoXi Micro
	
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
#ifndef LP5523_h
#define LP5523_h

#define LP5523_ADDR 0x34
#define LED1 0
#define LED2 1
#define LED3 2


enum led_selection_t { 	LED1_R = 0x0040, LED1_G = 0x0001, LED1_B = 0x0002, LED2_R = 0x0080, LED2_G = 0x0004, 
						LED2_B = 0x0008, LED3_R = 0x0100, LED3_G = 0x0010, LED3_B = 0x0020, GPO  = 0x8000 };
enum sign_t { UP, DOWN };
enum prescale_t { NOPRESCALE, PRESCALE };
enum step_time_t {	STEP_0ms5 = 1, STEP_1ms, STEP_1ms5, STEP_2ms, STEP_2ms5, STEP_3ms, STEP_3ms5, STEP_4ms, STEP_4ms5, STEP_5ms,
					STEP_5ms5, STEP_6ms, STEP_6ms5, STEP_7ms, STEP_7ms5, STEP_8ms, STEP_8ms5, STEP_9ms, STEP_9ms5, STEP_10ms,
					STEP_10ms5, STEP_11ms, STEP_11ms5, STEP_12ms, STEP_12ms5, STEP_13ms, STEP_13ms5, STEP_14ms, STEP_14ms5, STEP_15ms, 
					STEP_15ms5, STEP_16ms = 33, STEP_32ms, STEP_48ms, STEP_64ms, STEP_80ms, STEP_96ms, STEP_112ms, STEP_128ms, STEP_144ms,
					STEP_160ms, STEP_176ms, STEP_192ms, STEP_208ms, STEP_224ms, STEP_240ms, STEP_256ms, STEP_272ms, STEP_288ms, STEP_304ms,
					STEP_320ms, STEP_336ms, STEP_352ms, STEP_368ms, STEP_384ms, STEP_400ms, STEP_416ms, STEP_432ms, STEP_448ms, STEP_464ms,
					STEP_480ms, STEP_496ms };
enum engines_t { ENGINE1 = 1, ENGINE2 = 2, ENGINE3 = 4 };
enum eng_instr_t { DISABLE = 0b0000, LOAD = 0b0100, HOLD = 0b1000, STEP = 0b1001, FREE_RUN = 0b1010, EXEC_ONCE = 0b1011, HALT = 0b1100 };
enum variable_t { VARIABLE_A, VARIABLE_B, VARIABLE_C, VARIABLE_D }; //variables A & B are per engine, C is public and D is value from register address 0x3C
enum trigger_t { 	SEND_ENGINE1 = 0x0002, SEND_ENGINE2 = 0x0004, SEND_ENGINE3 = 0x0008, SEND_EXTERNAL = 0x0040, 
					WAIT_ENGINE1 = 0x0080, WAIT_ENGINE2 = 0x0100, WAIT_ENGINE3 = 0x0200, WAIT_EXTERNAL = 0x1000};
enum temperature_control_t {USER_DEFINED = 1, READ_ONCE = 4, CONTINUOUS_CONV = 6};

class LP5523 {
private:
	bool genCodePointerIncrement(void);
public:
	uint8_t genCodePointer; //pointer used in generating opcode
	uint16_t genCode[96]; //generated opcode
	bool op_ledMapping(uint16_t); //led mapping instruction for op code
	bool op_ramp(step_time_t, sign_t, uint8_t);
	bool op_ramp(uint8_t, sign_t, uint8_t, prescale_t); 
	bool op_set_pwm(uint8_t); 
	bool op_wait(step_time_t); 
	bool op_wait(uint8_t, prescale_t);
	bool op_mux_ld_start(uint8_t); 
	bool op_mux_map_start(uint8_t);
	bool op_mux_ld_end(uint8_t);
	bool op_mux_sel(led_selection_t);
	bool op_mux_sel(uint8_t);
	bool op_mux_clr(void);
	bool op_mux_map_next(void);
	bool op_mux_map_prev(void);
	bool op_mux_ld_next(void);
	bool op_mux_ld_prev(void);
	bool op_mux_ld_addr(uint8_t);
	bool op_mux_map_addr(uint8_t);
	bool op_rst(void);
	bool op_branch(uint8_t, uint8_t);
	bool op_int(void);
	bool op_end(bool, bool);
	bool op_trigger(trigger_t);
	bool op_variable_load(variable_t, uint8_t);
	bool op_variable_add(variable_t, uint8_t);
	bool op_variable_add(variable_t, variable_t, variable_t);
	bool op_variable_sub(variable_t, uint8_t);
	bool op_variable_sub(variable_t, variable_t, variable_t);
	bool op_jump_if_not_equal(uint8_t, variable_t, variable_t);
	bool op_jump_if_less(uint8_t, variable_t, variable_t);
	bool op_jump_if_greater(uint8_t, variable_t, variable_t);
	bool op_jump_if_equal(uint8_t, variable_t, variable_t);
	void resetCode(void); //resets currently generated op code
	void uploadCode(void); //uploads code to program memory, remember to set engines to load mode
	void engineStartAddress(uint8_t, uint8_t); //set engine program start address 0x00-0x5F
	void engineControl(uint8_t, eng_instr_t); //set engine mode
	void init(void); //initiate controller with basic settings
	void reset(void); //reset & turn off the controller
	void setCurrent(uint8_t); //output current control, 8-bits, 100µA step size
	void setLogDimm(bool); //enable or disable logarithmic dimming control
	void setLed(uint8_t, uint8_t, uint8_t, uint8_t); //set led red, green & blue PWM registers
	void tempControl(temperature_control_t); //set temperature control settings
	int8_t readTemperature(void);
	void writeTemperature(uint8_t); //set temperature, for testing or external sensor
	uint8_t readReg(uint8_t); //read register value
	void writeReg(uint8_t, uint8_t); //set register value
	};

#endif