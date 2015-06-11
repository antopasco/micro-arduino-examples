/*
  Led board example
  Controls leds brightness according to temperature.
  
  Datasheet: https://iprotoxi.fi/share/docs/lp5523.pdf
  By J. Piippo
  (c) iProtoXi Oy 2015-03-06
*/

#include <Wire.h>
#include <LP5523.h>

LP5523 led;


void setup()
{
  Wire.begin(); // join i2c bus
  Serial.begin(9600); // start serial
  led.reset(); // reset the controller
  led.init(); // initialize the controller
  led.setLogDimm(true); // set logarithmic dimming
  led.setCurrent(1); // limit current to 1mA
  led.tempControl(CONTINUOUS_CONV); //set temperature to be continuously measured
  delay(200);
}

void loop()
{
  if(Serial.available())
  {
    serialControl(); //type '?' into the serial for available serial controls / pause
    delay(100);
  }
  int8_t temperature = led.readTemperature(); //get temperature value from controller
  Serial.print(temperature); //print temperature
  Serial.println(" C");
  led.setLed(LED1, 0, 127+temperature, 0); //set green leds to temperature value + 127
  led.setLed(LED2, 0, 127+temperature, 0);
  led.setLed(LED3, 0, 127+temperature, 0);
  delay(100);
}

//Below is the serialControl function to easily test the unit
void serialControl(void)
{
  char serialData = Serial.read();
  bool pause = false; //pause serial with '?'
  if(serialData == '?')
  {
    pause = true;
    serialData = Serial.read();
  }
  if(serialData == 't')
  {
    int temperature_control = Serial.parseInt();
    switch(temperature_control)
    {
      case 1:
        led.tempControl(CONTINUOUS_CONV);
        break;
      case 2:
        led.tempControl(READ_ONCE);
        break;
      case 3:
        led.tempControl(USER_DEFINED);
        break;
      default:
        Serial.println("1 for continuous, 2 to read once, 3 to use user set temperature");
        break;
    }
  }
  else if(serialData == 's')
  {
    uint8_t setTemperature = Serial.parseInt();
    led.writeTemperature(setTemperature);
  }
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
    Serial.println("t<value> to change temperature control, s<value> to set temperature");
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
