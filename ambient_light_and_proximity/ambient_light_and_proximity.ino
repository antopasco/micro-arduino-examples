/*
  Ambient Light & Proximity Sensor
  Reads the data from ambient light & proximity unit and prints it to serial monitor.
  
  Datasheet: https://iprotoxi.fi/share/docs/bh1772glc_2-e.pdf
  By J. Piippo
  (c) iProtoXi 2015-03-02
*/

#include <Wire.h>
#include <BH1772.h>

BH1772 ALPsensor;

void setup()
{
  Wire.begin(); // join i2c bus
  Serial.begin(9600);  // start serial for output
  ALPsensor.reset(); //reset Ambient Light & Proximity sensor
  ALPsensor.ambientMode(ON_ALP); //turn on Ambient Light measurement
  ALPsensor.proximityMode(ON_ALP); //turn on Proximity measurement
  ALPsensor.irCurrent(IR_CURRENT_50mA); //set IR-led current to 50mA
  delay(200);
}
void loop()
{
  uint8_t proximityData;
  uint16_t ambientData;
  while(Serial.available())
    serialControl(Serial.read()); //type '?' into the serial for available serial controls / pause
  while(!Serial.available())
  {
    proximityData = ALPsensor.readProximity(); //read proximity value
    ambientData = ALPsensor.readAmbient(); //read ambient value
    Serial.print("proximity: "); 
    Serial.print(proximityData); //print proximity value
    Serial.print("\tambient: ");
    Serial.println(ambientData); //print ambient value
    delay(100);
  }
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
  if(serialData == 'p')
  {
    uint8_t mode = Serial.parseInt();
    switch(mode)
    {
      case 1:
      ALPsensor.proximityMode(ON_ALP);
      break;
      case 2:
      ALPsensor.proximityMode(TRIGGER_ALP);
      break;
      case 3:
      ALPsensor.proximityMode(DISABLE_ALP);
      break;
      case 4:
      ALPsensor.proximityMode(STANDBY_ALP);
      break;
      default:
      Serial.println("p<mode> to change modes: 1 = ON, 2 = TRIGGER, 3 = DISABLE, 4 = STANDBY");
      break;
    }
  }
  else if(serialData == 'a')
  {
    uint8_t mode = Serial.parseInt();
    switch(mode)
    {
      case 1:
        ALPsensor.ambientMode(ON_ALP);
        break;
      case 2:
        ALPsensor.ambientMode(TRIGGER_ALP);
        break;
      case 3:
        ALPsensor.ambientMode(DISABLE_ALP);
        break;
      case 4:
        ALPsensor.ambientMode(STANDBY_ALP);
        break;
      default:
        Serial.println("a<mode> to change modes: 1 = ON, 2 = TRIGGER, 3 = DISABLE, 4 = STANDBY");
        break;
    }
  }
  else if(serialData == 'i')
  {
    int current = Serial.parseInt();
    switch(current)
    {
      case 5:
        ALPsensor.irCurrent(IR_CURRENT_5mA);
        break;
      case 10:
        ALPsensor.irCurrent(IR_CURRENT_10mA);
        break;
      case 20:
        ALPsensor.irCurrent(IR_CURRENT_20mA);
        break;
      case 50:
        ALPsensor.irCurrent(IR_CURRENT_50mA);
        break;
      case 100:
        ALPsensor.irCurrent(IR_CURRENT_100mA);
        break;
      case 150:
        ALPsensor.irCurrent(IR_CURRENT_150mA);
        break;
      case 200:
        ALPsensor.irCurrent(IR_CURRENT_200mA);
        break;
      default:
        Serial.println("i<current> to change currents: 5, 10, 20, 50, 100, 150 or 200mA");
        break;
    }
  }
  else if(serialData == 's')
  {
    uint8_t sensitivity = Serial.parseInt();
    if(sensitivity>23 && sensitivity<255)
      ALPsensor.ambientSensitivity(sensitivity);
    else
      Serial.println("s<sensitivity> to change ambient sensitivity: 24-254");
  }
  else if(serialData == 'm')
  {
    uint8_t sensor = Serial.read();
    if(sensor == 'a')
    {
      int measRate = Serial.parseInt();
      switch(measRate)
      {
        case 0:
          ALPsensor.ambientMeasRate(MAX_MEAS_RATE);
          break;
        case 100:
          ALPsensor.ambientMeasRate(MEAS_RATE_100ms);
          break;
        case 200:
          ALPsensor.ambientMeasRate(MEAS_RATE_200ms);
          break;
        case 500:
          ALPsensor.ambientMeasRate(MEAS_RATE_500ms);
          break;
        case 1000:
          ALPsensor.ambientMeasRate(MEAS_RATE_1000ms);
          break;
        case 2000:
          ALPsensor.ambientMeasRate(MEAS_RATE_2000ms);
          break;
        default:
          Serial.println("Ambient meas rates: 100ms, 200ms, 500ms, 1000ms, 2000ms or 0 for max");
          break;
      }
      
    }
    else if(sensor == 'p')
    {
      int measRate = Serial.parseInt();
      switch(measRate)
      {
        case 0:
          ALPsensor.proximityMeasRate(MAX_MEAS_RATE);
          break;
        case 10:
          ALPsensor.proximityMeasRate(MEAS_RATE_10ms);
          break;
        case 20:
          ALPsensor.proximityMeasRate(MEAS_RATE_20ms);
          break;
        case 30:
          ALPsensor.proximityMeasRate(MEAS_RATE_30ms);
          break;
        case 50:
          ALPsensor.proximityMeasRate(MEAS_RATE_50ms);
          break;
        case 70:
          ALPsensor.proximityMeasRate(MEAS_RATE_70ms);
          break;
        case 100:
          ALPsensor.proximityMeasRate(MEAS_RATE_100ms);
          break;
        case 200:
          ALPsensor.proximityMeasRate(MEAS_RATE_200ms);
          break;
        case 500:
          ALPsensor.proximityMeasRate(MEAS_RATE_500ms);
          break;
        case 1000:
          ALPsensor.proximityMeasRate(MEAS_RATE_1000ms);
          break;
        case 2000:
          ALPsensor.proximityMeasRate(MEAS_RATE_2000ms);
          break;
        default:
          Serial.println("Proximity meas rates: 10ms, 20ms, 30ms, 50ms, 70ms, 100ms, 200ms, 500ms, 1000ms, 2000ms or 0 for max");
          break;
      }
	  
    }
    else
      Serial.println("type ma<value> to change ambient meas rate or mp<value> to change proximity meas rate");
  }
  else if(serialData == 't')
   {
     uint8_t sensor = Serial.read();
     if(sensor == 'a')
       ALPsensor.trigger(AMBIENT);
     else if(sensor == 'p')
       ALPsensor.trigger(PROXIMITY);
     else
       Serial.println("ta for ambient trigger, tp for proximity trigger");
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
    Serial.print(ALPsensor.readReg(readReg[0]*16+readReg[1])/16, HEX);
    Serial.println(ALPsensor.readReg(readReg[0]*16+readReg[1])%16, HEX);
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
    ALPsensor.writeReg(writeReg[0]*16+writeReg[1], writeReg[2]*16+writeReg[3]);
  }
  else
  {
    Serial.println("Type p, a, i, t for more info. s<uint8> to change ambient sensitivity");
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

