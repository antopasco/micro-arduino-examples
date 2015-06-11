/*
  Accelerometer
  Reads data the from accelerometer unit and prints it to serial monitor in millig.
  
  Datasheet: https://iprotoxi.fi/share/docs/LSM303DLM.pdf
  By J. Piippo
  (c) iProtoXi 2015-01-23
 */

#include <Wire.h>

#define ACC_ADDR      0x18 //Accelerometer address
#define CTRL_REG1_A   0x20 //Accelerometer Control Register 1 register address
#define CTRL_REG4_A   0x23 //Accelerometer Control Register 2 register address
#define NORMAL_PM     0x20 //CTRL_REG1_A normal powermode
#define DR_400        0x10 //CTRL_REG1_A Datarate 400Hz
#define DR_100        0x08 
#define EN_XYZ        0x07 //CTRL_REG1_A Enable x, y & z axes
#define SCALE_2G      0x00 //Maximum scale selection
#define SCALE_4G      0x10
#define SCALE_8G      0x30


void initAcc()
{
  writeAccRegs(CTRL_REG1_A, NORMAL_PM | DR_100 | EN_XYZ); //Set CTRL_REG1_A to normal power mode, 400Hz datarate and enable x, y, z axels 
  writeAccRegs(CTRL_REG4_A, SCALE_2G); //Set CTRL_REG4_A to use scale -2g to +2g
}

void writeAccRegs(unsigned char reg, unsigned char value)
{
  Wire.beginTransmission(ACC_ADDR); //begin transmission to accelerometer
  Wire.write(&reg, sizeof(reg)); //send register address
  Wire.write(&value, sizeof(value)); //send register value
  Wire.endTransmission();
}

void readAcc(int &x, int &y, int &z, int &resultant)
{
  float xAxis; //float so calculation won't fail later
  float yAxis;
  float zAxis;
  
  Wire.beginTransmission(ACC_ADDR); //begin transmission to accelerometer
  Wire.write(0xA8); //request all sensor data
  
  if(Wire.requestFrom(ACC_ADDR, 6) < 6) //request & check we got all the 3x2bytes of data
    Serial.println("Error: Not enough data");

  xAxis = Wire.read(); //read first byte
  xAxis = 256*Wire.read() + xAxis; //read second byte, multiply it by 2^8 and sum it with previous byte
  yAxis = Wire.read();
  yAxis = 256*Wire.read() + yAxis;
  zAxis = Wire.read();
  zAxis = 256*Wire.read() + zAxis;
  
  Wire.endTransmission(); //reset wire connection

   x = xAxis*1000/(1024*16); //transform data to millig, for SCALE_2G xAxis*1000/(1024*16),
   y = yAxis*1000/(1024*16); //for SCALE_4G xAxis*1000/(1024*8),
   z = zAxis*1000/(1024*16); //for SCALE_8G xAxis*1000/(1024*4)
   resultant = sqrt(xAxis*xAxis+yAxis*yAxis+zAxis*zAxis)*1000/(1024*16); //resultant acceleration in millig
}

void setup()
{
  Wire.begin();        // join i2c bus
  Serial.begin(9600);  // start serial for output
  initAcc();           // init accelerometer
  delay(1000);
}

void loop()
{
    int x;
    int y;
    int z;
    int resultant;
    readAcc(x, y, z, resultant); //read axes data and return it to x, y, z, resultant variables respectively
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

