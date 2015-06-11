/*
  Proximity interrupt controlled led engine example
  By J. Piippo
  (c) iProtoXi 2015-03-04
 */

#include <Wire.h>
#include <BH1772.h>
#include <LP5523.h>
#include <MsTimer2.h>
#include <avr/power.h>
#include <avr/sleep.h>

BH1772 ALPsensor;
LP5523 led;

volatile bool pin11Value = 1;
volatile bool gotInterrupt = true;
volatile bool time10msPassed = true;
int currentEngine = 3;

void setup()
{
  Wire.begin(); // join i2c bus
  
  led.reset(); //reset led controller
  led.init(); //initiate led controller
  led.setLogDimm(true); //set logarithmic dimming
  led.setCurrent(1); //set max current to 0.1mA
  
  led.resetCode(); //reset opcode
  opcodeRainbow(); //generate rainbow-opcode
  opcodeHeartBeat(); //generate heartbeat-opcode
  opcodeBreath(); //generate breath-opcode
  led.uploadCode(); //upload generated code
  
  ALPsensor.reset(); //reset ambient light & proximity sensor
  ALPsensor.ambientMode(DISABLE_ALP); //set ambient measurement off
  ALPsensor.proximityMode(ON_ALP); //set proximity measurement on
  ALPsensor.proximityMeasRate(MEAS_RATE_10ms);
  ALPsensor.irCurrent(IR_CURRENT_50mA); //set IR-led current to 50mA
  ALPsensor.proximityInterruptTresholdHigh(0x0F); //set proximity interrupt treshold to 0x0F
  ALPsensor.selectInterrupt(PROXIMITY); //enable proximity interrupt
  
  noInterrupts();
  setPCinterrupt(); //enable pin change interrupt on pin 11
  MsTimer2::set(10, timerInterrupt); // 10ms period
  interrupts();
  delay(1000);
}
void opcodeHeartBeat(void)
{ 
  uint8_t codeStartAddress = 0x20;
  uint8_t ledMappingAddress = 0x38;
  
  led.genCodePointer = codeStartAddress; //start generating code from 0x20
  led.op_mux_map_addr(ledMappingAddress); //get led map from program memory slot 0x38
  led.op_set_pwm(0); //shut down all leds
  led.op_mux_map_addr(ledMappingAddress+1); //get mapping from 0x39
  led.op_set_pwm(105); //set red leds pwm to 105
  led.op_mux_map_next(); //get next led mapping (from 0x3A)
  led.op_ramp(STEP_6ms, UP, 35); //increase pwm 35 times with 6ms interval
  led.op_ramp(STEP_1ms5, DOWN, 35); //decrease pwm 35 times with 1.5ms interval
  led.op_wait(STEP_15ms); //wait 15ms
  led.op_mux_map_next(); 
  led.op_ramp(STEP_3ms, DOWN, 15);
  led.op_ramp(STEP_0ms5, UP, 100);
  led.op_ramp(STEP_0ms5, DOWN, 120);
  led.op_ramp(STEP_1ms, UP, 35);
  led.op_wait(STEP_15ms);
  led.op_mux_map_next();
  led.op_ramp(STEP_2ms, UP, 39);
  led.op_ramp(STEP_1ms, DOWN, 39);
  led.op_wait(STEP_368ms);
  led.op_branch(0, 2); //loop from 3rd instruction infinitely
 
  led.genCodePointer = ledMappingAddress; //start generating code to 0x38
  led.op_ledMapping(LED1_R | LED1_G | LED1_B | LED2_R | LED2_G | LED2_B | LED3_R | LED3_G | LED3_B); //map all leds
  led.op_ledMapping(LED1_R | LED2_R | LED3_R); //map red leds
  led.op_ledMapping(LED3_R);
  led.op_ledMapping(LED2_R);
  led.op_ledMapping(LED1_R);
  
  led.engineStartAddress(ENGINE2, codeStartAddress); //set engine 2 start address to 0x20
  led.engineControl(ENGINE2, LOAD); //set engine 3 to load mode
}
void opcodeBreath(void)
{ 
  uint8_t codeStartAddress = 0x40;
  uint8_t ledMappingAddress = 0x55;
  
  led.genCodePointer = codeStartAddress; //start generating code to 0x40
  led.op_mux_map_addr(ledMappingAddress+1);
  led.op_set_pwm(0);
  led.op_mux_map_addr(ledMappingAddress); //get led mapping from program memory slot 0x55
  led.op_set_pwm(100); //set pwm to 100
  led.op_mux_map_addr(ledMappingAddress); 
  led.op_ramp(STEP_15ms5, UP, 1); //increase pwm once with 15.5ms interval
  led.op_mux_sel(LED2_B); //select D4
  led.op_ramp(STEP_16ms, UP, 2); //increase pwm twice with 16ms interval
  led.op_branch(41, 0x04); //repeat from 0x04 41 times
  led.op_mux_map_addr(ledMappingAddress); 
  led.op_ramp(STEP_16ms, DOWN, 1); //decrease pwm once with 16ms interval
  led.op_mux_sel(LED2_B); //select D4
  led.op_ramp(STEP_32ms, DOWN, 2); //decrease pwm twice with 32ms interval
  led.op_branch(41, 0x09);  //repeat from 0x09 41 times
  led.op_mux_map_addr(ledMappingAddress);
  led.op_ramp(STEP_32ms, DOWN, 8); //decrease pwm 8 times with 32ms interval
  led.op_ramp(STEP_32ms, UP, 8); //increase pwm 8 times with 32ms interval
  led.op_branch(0, 0x02);
  
  led.genCodePointer = ledMappingAddress; //start generating code to 0x55
  led.op_ledMapping(LED1_B | LED2_B | LED3_B); //map blue leds
  led.op_ledMapping(LED1_R | LED1_G | LED1_B | LED2_R | LED2_G | LED2_B | LED3_R | LED3_G | LED3_B);
  
  led.engineStartAddress(ENGINE3, 0x40); //set engine 3 start address to 0x40
  led.engineControl(ENGINE3, LOAD); //set engine 3 to load mode
}
void opcodeRainbow(void)
{
  uint8_t codeStartAddress = 0x00;
  uint8_t ledMappingAddress = 0x1E;
  
  led.genCodePointer = codeStartAddress; //start generating code to 0x40
  led.op_mux_map_addr(ledMappingAddress);//select all leds
  led.op_set_pwm(0); //shut down all leds
  led.op_mux_map_addr(ledMappingAddress+1); //select led1 red & blue
  led.op_ramp(STEP_1ms, UP, 255); //power up led1 red & blue
  led.op_mux_sel(LED1_R); //select led1 red
  led.op_ramp(STEP_0ms5, DOWN, 5); //decrease led1 red pwm 5 times
  led.op_mux_sel(LED2_B); //select led2 blue
  led.op_ramp(STEP_0ms5, UP, 5); //increase led2 blue pwm 5 times
  led.op_branch(51, 4); //repeat last four instructions 51 times
  led.op_mux_sel(LED1_B); //select led1 blue
  led.op_ramp(STEP_0ms5, DOWN, 5); //decrease led1 blue pwm 5 times
  led.op_mux_sel(LED2_G);  //select led2 green
  led.op_ramp(STEP_0ms5, UP, 5); //decrease led2 green pwm 5 times
  led.op_branch(51, 9); //repeat last four instructions 51 times
  led.op_mux_sel(LED2_B); //select led2 blue
  led.op_ramp(STEP_0ms5, DOWN, 5); //decrease led2 blue pwm 5 times
  led.op_mux_sel(LED3_G);  //select led3 green
  led.op_ramp(STEP_0ms5, UP, 5); //increase led3 green pwm 5 times
  led.op_branch(51, 14); //repeat last four instructions 51 times
  led.op_mux_sel(LED2_G); //select led2 green
  led.op_ramp(STEP_0ms5, DOWN, 5); //decrease led2 green pwm 5 times
  led.op_mux_sel(LED3_R);  //select led3 red
  led.op_ramp(STEP_0ms5, UP, 5); //increase led3 red pwm 5 times
  led.op_branch(51, 19); //repeat last four instructions 51 times
  led.op_mux_sel(LED3_G); //select led3 green
  led.op_ramp(STEP_0ms5, DOWN, 255); //power down led3 green
  led.op_mux_sel(LED3_R); //select led3 red
  led.op_ramp(STEP_1ms, DOWN, 255); //power down led3 red
  
  led.genCodePointer = ledMappingAddress; //start generating code to 0x20
  led.op_ledMapping(LED1_R | LED1_G | LED1_B | LED2_R | LED2_G | LED2_B | LED3_R | LED3_G | LED3_B); //map all leds
  led.op_ledMapping(LED1_R | LED1_B); //map led3 red & blue
  
  led.engineStartAddress(ENGINE1, codeStartAddress); //set engine 1 start address to 0x00
  led.engineControl(ENGINE1, LOAD); //set engine 1 to load mode
}
void setPCinterrupt(void)
{
  pinMode(11, INPUT);
  PCIFR &= ~(1 << PCIE0); 
  PCICR |= (1 << PCIE0);
  PCMSK0 |= (1 << PCINT7);
}
void timerInterrupt(void)
{
  time10msPassed = true;
}

ISR(PCINT0_vect) //interrupt handler for pin change interrupt
{
  gotInterrupt = true;
  pin11Value = digitalRead(11);
}
void goToSleep()
{
  set_sleep_mode(SLEEP_MODE_IDLE);  //set sleep mode to idle
  sleep_enable(); //enable sleep
  sleep_mode(); //go to sleep, continues from here
  sleep_disable(); //disable sleep
}
void loop()
{
  if(gotInterrupt)
  {
    gotInterrupt = false;
    if(pin11Value) //check if pin 11 was high on interrupt
    {
      currentEngine = 3;
      MsTimer2::stop(); //stop timer
      led.engineControl(ENGINE2, DISABLE); //disable engine 1 & 2
      led.engineControl(ENGINE1, DISABLE);
      led.engineStartAddress(ENGINE3, 0x40); //reset engine 3 address
      led.engineControl(ENGINE3, FREE_RUN); //start engine 3
    }
    else
      MsTimer2::start(); //start timer
  }
  else if(!pin11Value && time10msPassed) //check if pin 11 was low and 10ms has passed since last proximity check
  {
    time10msPassed = false;
    uint8_t proximityValue = ALPsensor.readProximity(); //read proximity value
    if(proximityValue<51 && (currentEngine != 2)) //check if proximity value is less than 51 and engine2 is not currently running
    {
      currentEngine = 2;
      led.engineControl(ENGINE1, DISABLE); //disable engine 1 & 3, start engine 2
      led.engineControl(ENGINE3, DISABLE);
      led.engineStartAddress(ENGINE2, 0x20);
      led.engineControl(ENGINE2, FREE_RUN);
    }
    else if(proximityValue>50 && (currentEngine != 1)) //check if proximity value is more than 50 and engine 1 is not currently running 
    {
      currentEngine = 1;
      led.engineControl(ENGINE2, DISABLE); //disable engine 2 & 3, start engine 1
      led.engineControl(ENGINE3, DISABLE);
      led.engineStartAddress(ENGINE1, 0x00);
      led.engineControl(ENGINE1, FREE_RUN);
    }
    else
      goToSleep();
  }
  else
    goToSleep();
}
