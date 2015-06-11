/*
  Advanced led board engine example
  Multiple engine code examples selectable through serial.
  
  Datasheet: https://iprotoxi.fi/share/docs/lp5523.pdf
  By J. Piippo
  (c) iProtoXi Oy 2015-02-13
*/

#include <Wire.h>
#include <LP5523.h>

LP5523 led;
uint8_t turnOn = 0;


void setup()
{
  Wire.begin(); // join i2c bus
  Serial.begin(9600); // start serial
  led.reset(); // reset the controller
  led.init(); // initialize the controller
  led.setLogDimm(true); // set logarithmic dimming
  led.setCurrent(1); // limit current to 1mA
  led.setLed(0, 255, 0, 0); //power on red
  led.setLed(1, 0, 255, 0); //power on green
  led.setLed(2, 0, 0, 255); //power on blue
  delay(200);
}

void testLeds(void)
{
  Serial.println("testing leds");
  led.reset();
  led.init();
  led.setCurrent(10);
  led.setLogDimm(true);
  for(int i=0; i<3; i++)
  {
    Serial.print("setting led ");Serial.println(i);
    led.setLed(i, 255, 0, 0); //set led i red pwm to 255
    delay(200);
    led.setLed(i, 0, 255, 0); //set led i green pwm to 255
    delay(200);
    led.setLed(i, 0, 0, 255);  //set led i blue pwm to 255
    delay(200);
    led.setLed(i, 0, 0, 0);    //shut down led i all colors
    delay(200);
  }
}
void opcodeHeartBeat(void)
{ 
/*
  Hearbeat opcode generating function
  Raw usage of opcode generating functions
*/
  Serial.println("1. Heart beat");
  led.writeReg(0x1c, 105); //set red leds pwm to 105
  led.writeReg(0x1d, 105);
  led.writeReg(0x1e, 105);
  led.genCodePointer = 0x30; //start generating code from 0x30
  led.op_mux_map_addr(0x40); //get led map from program memory slot 0x20
  led.op_ramp(12, UP, 35, NOPRESCALE); //ramp up, 12 step times (0.5ms*12 = 6ms), 35 times, no prescale
  led.op_ramp(3, DOWN, 35, NOPRESCALE); //ramp down, 3 step times (0.5ms*3 = 1.5ms), 35 times, no prescale
  led.op_wait(30, NOPRESCALE); //wait 30 step times (0.5ms*30 = 15ms), no prescale
  led.op_mux_map_next(); //get led map from next program memory slot (0x21)
  led.op_ramp(6, DOWN, 15, NOPRESCALE);
  led.op_ramp(1, UP, 100, NOPRESCALE);
  led.op_ramp(1, DOWN, 120, NOPRESCALE);
  led.op_ramp(2, UP, 35, NOPRESCALE);
  led.op_wait(30, NOPRESCALE);
  led.op_mux_map_next();
  led.op_ramp(4, UP, 39, NOPRESCALE);
  led.op_ramp(2, DOWN, 39, NOPRESCALE);
  led.op_wait(23, PRESCALE); //wait 23*32 step times (0.5ms*23*32 = 368ms)
 
  led.genCodePointer = 0x40; //move pointer to 0x20 (mux map address)
  led.op_ledMapping(0x0100); //map D9 pin
  led.op_ledMapping(0x0080); //map D8 pin
  led.op_ledMapping(0x0040); //map D7 pin
  
  led.engineStartAddress(ENGINE2, 0x30); //set engine 2 start address to 0x30
  turnOn = turnOn | ENGINE2; //set engine 2 to be turned on later
  if(Serial.available()) //if there is more coming from serial, read it before uploading code & turning on engines 
    serialControl(Serial.read());
}

void opcodeBreath(void) //
{ 
/*
  Breath opcode generating function.
  Basic example for opcode generation.
*/
  Serial.println("2. Breath");
  led.genCodePointer = 0x00;
  led.op_mux_map_addr(0x10); //get led mapping from program memory slot 0x10
  led.op_set_pwm(100); //set pwm to 100  
  led.op_ramp(STEP_15ms5, UP, 125); //increase pwm 125 times with 15.5ms interval
  led.op_ramp(STEP_32ms, DOWN, 125); //decrease pwm 125 times with 15.5ms interval
  led.op_wait(STEP_480ms); //wait 480ms
  led.op_branch(0, 2); //loop from 3rd instruction infinitely
  
  led.genCodePointer = 0x10; //set code pointer to 0x10
  led.op_ledMapping(LED1_B | LED2_B  | LED3_B); //map blue leds
  
  led.engineStartAddress(ENGINE1, 0x00); //set engine 1 start address to 0x00
  turnOn = turnOn | ENGINE1; //set engine 1 to be turned on later
  if(Serial.available()) //if there is more coming from serial, read it before uploading code & turning on engines
    serialControl(Serial.read());
}

void opcodeBreath2(void) //
{ 
/*
  Breath 2 opcode generating function
  Example for branch usage.
*/
  Serial.println("3. Breath 2");
  
  uint8_t startAddress = 0x00;
  uint8_t mapAddress = 0x10;
  
  led.genCodePointer = 0x00;
  led.op_mux_map_addr(mapAddress); //get led mapping from program memory slot 0x10
  led.op_set_pwm(100); //set pwm to 100
  led.op_mux_map_addr(mapAddress); //get led mapping from program memory slot 0x10
  led.op_ramp(STEP_15ms5, UP, 1); //increase pwm once with 15.5ms interval
  led.op_mux_sel(LED2_B); //select D4 (middle blue)
  led.op_ramp(STEP_16ms, UP, 2); //increase pwm twice with 16ms interval
  led.op_branch(41, 2); //repeat from 3rd instruction 41 times
  led.op_mux_map_addr(mapAddress); //get led mapping from program memory slot 0x10
  led.op_ramp(STEP_16ms, DOWN, 1); //decrease pwm once with 16ms interval
  led.op_mux_sel(LED2_B); //select D4 (middle blue)
  led.op_ramp(STEP_32ms, DOWN, 2); //decrease pwm twice with 32ms interval
  led.op_branch(41, 7);  //repeat from 8th instruction 41 times
  led.op_mux_map_addr(mapAddress); //get led mapping from program memory slot 0x10
  led.op_ramp(STEP_32ms, DOWN, 8); //decrease pwm 8 times with 32ms interval
  led.op_ramp(STEP_32ms, UP, 8); //increase pwm 8 times with 32ms interval
  led.op_branch(0, 2);  //repeat from 3rd instruction infinitely
  
  led.genCodePointer = mapAddress; //set code pointer to 0x10
  led.op_ledMapping(LED1_B | LED2_B | LED3_B); //map blue leds
  
  led.engineStartAddress(ENGINE1, startAddress);
  turnOn = turnOn | ENGINE1; //set engine 1 to be turned on later
  if(Serial.available()) //if there is more coming from serial, read it before uploading code & turning on engines
    serialControl(Serial.read());
}
void opcodeBeat(void)
{
/*
  Beat opcode generating function.
  Basic example for branch usage.
*/
  Serial.println("4. Beat");
  
  uint8_t startAddress = 0x50; 
  uint8_t ledAddress = 0x58;
  
  led.genCodePointer = startAddress; //start generating code from startAddress
  led.op_mux_map_addr(ledAddress); //get led mapping from program memory slot 0x50
  led.op_set_pwm(210); //set pwm to 210
  led.op_ramp(STEP_0ms5, UP, 15); //increase mapped leds pwm 15 times with 0.5ms interval
  led.op_ramp(STEP_0ms5, DOWN, 225); //decrease mapped leds pwm 225 times with 0.5ms interval
  led.op_mux_map_next(); //read next mapping
  led.op_wait(STEP_256ms); //wait 256ms
  led.op_branch(3, 1); //repeat from 2nd istruction 3 times
  
  led.genCodePointer = ledAddress; //set pointer to program memory address 0x30
  led.op_ledMapping(LED3_G); //map left green led
  led.op_ledMapping(LED3_G | LED2_G); //map left green & middle green leds
  led.op_ledMapping(LED3_G); 
  led.op_ledMapping(LED3_G | LED1_G); //map left green & right green leds
  
  led.engineStartAddress(ENGINE3, startAddress); //set engine 3 start address to startAddress
    turnOn = turnOn | ENGINE3; //set engine 1 to be turned on later
  if(Serial.available()) //if there is more coming from serial, read it before uploading code & turning on engines
    serialControl(Serial.read());
}
void opcodeRainbow(void)
{
/*
  Rainbow opcode generating function.
  Basic example for branch usage.
*/
  Serial.println("5. Rainbow");
  led.genCodePointer = 0x00;
  led.op_mux_map_addr(0x20);//select all leds
  led.op_set_pwm(0); //shut down all leds
  led.op_mux_map_addr(0x21); //select led1 red & blue
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
  
  led.genCodePointer = 0x20;
  led.op_ledMapping(LED1_R | LED1_G | LED1_B | LED2_R | LED2_G | LED2_B | LED3_R | LED3_G | LED3_B); //map all leds
  led.op_ledMapping(LED1_R | LED1_B); //map led3 red & blue
  
  led.engineStartAddress(ENGINE1, 0x00); //set engine 1 start address to 0x00
  turnOn = turnOn | ENGINE1; //set engine 1 to be turned on
  if(Serial.available()) //if there is more coming from serial, read it before uploading code & turning on engines
    serialControl(Serial.read());
}
void opcodeMultiengine(void)
{
/*
  All 3 engines using opcode generating function.
  Basic example for using multiple engines.
*/
  Serial.println("6. All engines");
  led.engineStartAddress(ENGINE1, 0x00); //generate engine 1 operational code from 0x00
  led.genCodePointer = 0x00;
  led.op_mux_sel(7);
  led.op_ramp(STEP_1ms, UP, 255);
  led.op_ramp(STEP_1ms, DOWN, 255);
  led.op_mux_sel(8);
  led.op_ramp(STEP_1ms5, UP, 255);
  led.op_ramp(STEP_1ms5, DOWN, 255);
  led.op_mux_sel(9);
  led.op_ramp(STEP_1ms, UP, 255);
  led.op_ramp(STEP_1ms, DOWN, 255);
  
  led.engineStartAddress(ENGINE2, 0x10); //generate engine 2 operational code from 0x10
  led.genCodePointer = 0x10;
  led.op_mux_sel(2);
  led.op_ramp(STEP_1ms5, UP, 255);
  led.op_ramp(STEP_1ms5, DOWN, 255);
  led.op_mux_sel(4);
  led.op_ramp(STEP_1ms, UP, 255);
  led.op_ramp(STEP_1ms, DOWN, 255);
  led.op_mux_sel(6);
  led.op_ramp(STEP_1ms5, UP, 255);
  led.op_ramp(STEP_1ms5, DOWN, 255);
  
  led.engineStartAddress(ENGINE3, 0x20); //generate engine 3 operational code from 0x20
  led.genCodePointer = 0x20;
  led.op_mux_map_addr(0x30);
  led.op_set_pwm(255);
  led.op_mux_sel(1);
  led.op_ramp(STEP_1ms5, DOWN, 255);
  led.op_ramp(STEP_1ms5, UP, 255);
  led.op_mux_sel(3);
  led.op_ramp(STEP_1ms, DOWN, 255);
  led.op_ramp(STEP_1ms, UP, 255);
  led.op_mux_sel(5);
  led.op_ramp(STEP_1ms5, DOWN, 255);
  led.op_ramp(STEP_1ms5, UP, 255);
  led.op_mux_sel(3);
  led.op_ramp(STEP_1ms, DOWN, 255);
  led.op_ramp(STEP_1ms, UP, 255);
  led.op_branch(0, 0x02);
  
  led.genCodePointer = 0x30;
  led.op_ledMapping(LED1_G | LED2_G | LED3_G);
  
  turnOn = ENGINE1 | ENGINE2 | ENGINE3; //set all engines to be turned on
}

void loop()
{
  while(Serial.available())
  {
    serialControl(Serial.read()); //type '?' into the serial for available serial controls 
    if(turnOn)
    {
      led.engineControl(turnOn, LOAD); //set selected engines to load mode
      led.uploadCode(); //upload code
      led.engineControl(turnOn, FREE_RUN); //set selected engines to free run
    }
    turnOn = 0;
  }
  delay(50);
}

//Below is the serialControl function to easily test the unit
void serialControl(uint8_t serialData)
{
  bool pause = false; //pause serial with '?'
  if(serialData == '?')
  {
    pause = true;
    serialData = Serial.read();
  }
  if(serialData == '1' || serialData == '2' || serialData == '3' || serialData == '4' || serialData == '5' || serialData == '6')
    {
      led.reset();
      led.init();
      led.setLogDimm(true);
      led.setCurrent(1);
      led.resetCode(); //reset opcode
    }
  if(serialData == '0')
    testLeds();
  else if(serialData == '1')
    opcodeHeartBeat();
  else if(serialData == '2')
    opcodeBreath(); 
  else if(serialData == '3')
    opcodeBreath2();
  else if(serialData == '4')
    opcodeBeat();
  else if(serialData == '5')
    opcodeRainbow();
  else if(serialData == '6')
    opcodeMultiengine();
  else if(serialData == 'r')
  {
    uint8_t readReg[2];
    for(int i = 0; i < 2; i++)
    {
      readReg[i] = Serial.read();
      if(readReg[i] > 96)
        readReg[i] = readReg[i] - 87;
      else if(readReg[i] > 64)
        readReg[i] = readReg[i] - 55;
      else
        readReg[i] = readReg[i] - 48;
    }
    Serial.println(led.readReg(readReg[0]*16+readReg[1]), HEX);
  }
  else if(serialData == 'w')
  {
    uint8_t writeReg[4];
    for(int i = 0; i < 4; i++)
    {
      writeReg[i] = Serial.read();
      if(writeReg[i] > 96)
        writeReg[i] = writeReg[i] - 87;
      else if(writeReg[i] > 64)
        writeReg[i] = writeReg[i] - 55;
      else
        writeReg[i] = writeReg[i] - 48;
    }
    led.writeReg(writeReg[0]*16+writeReg[1], writeReg[2]*16+writeReg[3]);
  }
  else
  {
    Serial.println("Type 1, 2, 3, 4, 5 or 6 to execute example programs or combination e.g. '134'.");
    Serial.println("To read register value use r+register address in hex e.g. 'r00' (read reg 00)");
    Serial.println("To write register value use w+register address+value in hex e.g. 'w16FF' (write FF to reg 16)");
  }
  if(pause)
  {
    Serial.println("Press any key to continue..");
    while(!Serial.available())
      delay(10);
  }
}
