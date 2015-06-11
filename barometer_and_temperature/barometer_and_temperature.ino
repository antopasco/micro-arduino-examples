/*
  Barometer & Temperature example
  Reads the data from LPS331-sensor and prints it to serial monitor in mBar & celsius.

  Datasheet: https://iprotoxi.fi/share/docs/DM00036196.pdf
  By J. Piippo
  (c) iProtoXi Oy 2015-03-04
*/

#include <Wire.h>
#include <LPS331.h>

LPS331 barometer;

void setup()
{
  Wire.begin();  // join i2c bus
  Serial.begin(9600);  // start serial for output
  barometer.reset();  //reset barometer
  barometer.init();  //initiate barometer
  barometer.measureRate(MEAS_RATE_12Hz5);  //set measure rate to 12.5Hz
  barometer.pressureDelta(false);  //disable delta pressure
  delay(1000);
}

void loop()
{
  int32_t pressureRaw;
  int16_t temperatureRaw;
  float pressure;
  float temperature;
  if(Serial.available())
    serialControl(); //type '?' into the serial for available serial controls / pause
  while(!Serial.available())
  {
    pressureRaw = barometer.readPresRaw(); //read raw pressure
    temperatureRaw = barometer.readTempRaw(); //read raw temperature
    pressure = barometer.mBarPress(pressureRaw); //calculate mbar form raw pressure data
    temperature = barometer.celsiusTemp(temperatureRaw); //calculate celsius from raw temperature data
    delay(100); //delay to slow down process
    Serial.print("Pres:\t");
    Serial.print(pressure); //print pressure
    Serial.print(" mBar\t Temp:");
    Serial.print(temperature); //print temperature
    Serial.println(" C");
  }
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
  if(serialData == 'm')
  {
    int measureRate = Serial.parseInt();
    switch(measureRate)
    {
      case -1:
        barometer.measureRate(ONE_SHOT);
        break;
      case 1:
        barometer.measureRate(MEAS_RATE_1Hz);
        break;
      case 7:
        barometer.measureRate(MEAS_RATE_7Hz);
        break;
      case 12:
        barometer.measureRate(MEAS_RATE_12Hz5);
        break;
      case 25:
        barometer.measureRate(MEAS_RATE_25Hz);
        break;
      default:
        Serial.println("Possible measure rates: 1Hz, 7Hz, 12.5Hz, 25Hz and -1 for one shot");
      while(Serial.available())
        Serial.read();
    }
  }
  else if(serialData == 'a')
    barometer.autoZero();
  else if(serialData == 'd')
  {
    char deltaEnable = Serial.read();
    if(deltaEnable == '0')
      barometer.pressureDelta(false);
    else if(deltaEnable == '1')
      barometer.pressureDelta(true);
    else
      Serial.println("d0 to disable, d1 to enable pressure delta");
  }
  else if(serialData == '0')
    barometer.oneShotMeasure();
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
    Serial.print(barometer.readReg(readReg[0]*16+readReg[1])/16, HEX);
    Serial.println(barometer.readReg(readReg[0]*16+readReg[1])%16, HEX);
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
    barometer.writeReg(writeReg[0]*16+writeReg[1], writeReg[2]*16+writeReg[3]);
  }
  else
  {
    Serial.println("m<value> to change measure rate. d0 or d1 to enable pressure delta.");
    Serial.println("a to get auto zero value. 0 to get new pressure value on one shot mode."); 
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
