/*
  Motion sensor
  Reads the data from motion sensor unit and prints it to serial monitor in millig.
  
  Datasheet: https://iprotoxi.fi/share/docs/LSM303DLM.pdf
  By J. Piippo
  (c) iProtoXi Oy 2015-01-26
*/

#include <Wire.h>
#include <LSM303DLM.h>

LSM303 accelerometer;

void setup()
{
  Wire.begin(); // join i2c bus
  Serial.begin(9600); // start serial
  accelerometer.init(); // init accelerometer
  accelerometer.setODR(ODR_100HZ); //set output data rate to 100Hz
  delay(1000);
}

void loop()
{
  if(Serial.available())
  {
    serialControl(); //type '?' into the serial for available serial controls / pause
    delay(100);
  }
  int x;
  int y;
  int z;
  unsigned int resultant; //resultant max value 56755 thus unsigned int
  accelerometer.readRawXYZ(x, y, z); //read axes data and return it to x, y & z variables respectively
  accelerometer.mgScale(x, y, z); //scale to millig
  resultant = accelerometer.getResultant(x, y, z); //calculate resultant vector
  delay(100); //delay to slow down process
  Serial.print("x: "); //print everything
  Serial.print(x);
  Serial.print(" mg \ty: ");
  Serial.print(y);
  Serial.print(" mg \tz: ");
  Serial.print(z);
  Serial.print(" mg \ttotal: ");
  Serial.print(resultant);
  Serial.println(" mg");
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
  if(serialData == 'O' | serialData == 'o')
  {
    int dataRate = Serial.parseInt();
    switch(dataRate)
    {
    case 1000:
      accelerometer.setODR(ODR_1000HZ);
      break;
    case 400:
      accelerometer.setODR(ODR_400HZ);
      break;
    case 100:
      accelerometer.setODR(ODR_100HZ);
      break;
    case 50:
      accelerometer.setODR(ODR_50HZ);
      break;
    case 10:
      accelerometer.setODR(ODR_10HZ);
      break;
    case 5:
      accelerometer.setODR(ODR_5HZ);
      break;
    case 2:
      accelerometer.setODR(ODR_2HZ);
      break;
    case 1:
      accelerometer.setODR(ODR_1HZ);
      break;
    case 0:
      accelerometer.setODR(ODR_0HZ5);
      break;
    default:
      Serial.println("Invalid data rate, possible 1000Hz, 400Hz, 100Hz, 50Hz, 10Hz, 5Hz, 2Hz, 1Hz, and 0 for 0.5Hz");
      break;
    }
    while(Serial.available())
      Serial.read();
  }
  else if(serialData == 'G' | serialData == 'g')
  {
    int scale = Serial.read();
    if(scale == '2') //if G2
      accelerometer.setScale(SCALE_2g);
    else if(scale == '4') //if G4
      accelerometer.setScale(SCALE_4g);
    else if(scale == '8') //if G8
      accelerometer.setScale(SCALE_8g);
    if(scale - 48 == accelerometer.getScale()) //check scale is succesfully set
      Serial.println("Scale set");
    else
      Serial.println("Invalid scale, possible 2, 4 or 8");
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
    Serial.println(accelerometer.readReg(readReg[0]*16+readReg[1]), HEX);
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
    accelerometer.writeReg(writeReg[0]*16+writeReg[1], writeReg[2]*16+writeReg[3]);
  }
  else
  {
    Serial.println("O<datarate> to change Output Data Rate, G2, G4 or G8 to change g scale");
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

