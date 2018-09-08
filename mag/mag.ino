/**
 * @author Yasara JLP
 * MAG3110 magnetometer - I2C
 * 
 */


#include<Wire.h>

// MAG3110 I2C address is 0x0E(14)
#define Addr 0x0E

void setup() 
{
  // Initialise I2C communication as MASTER 
  Wire.begin();
  // Initialise serial communication, set baud rate = 9600
  Serial.begin(19200);
  
  // Start I2C Transmission
  Wire.beginTransmission(Addr);
  // Select control register-1
  Wire.write(0x10);
  // Set active mode enabled
  Wire.write(0x01);
  // Stop I2C Transmission
  Wire.endTransmission();
  delay(300);
}

void loop() 
{
  unsigned int data[6];
  
  // Start I2C Transmission
  Wire.beginTransmission(Addr);
  // Select data register
  Wire.write(0x01);
  // Stop I2C Transmission
  Wire.endTransmission();

  // Request 6 bytes of data
  Wire.requestFrom(Addr, 6);

  // Read 6 bytes of data
  // xMag lsb, xMag msb, yMag lsb, y Mag msb, zMag lsb, zMag msb
  if(Wire.available() == 6)
  {
    data[0] = Wire.read();
    data[1] = Wire.read();
    data[2] = Wire.read();
    data[3] = Wire.read();
    data[4] = Wire.read();
    data[5] = Wire.read();
  }
  
  // Convert the data
  int xMag = ((data[1] * 256) + data[0]);
  int yMag = ((data[3] * 256) + data[2]);
  int zMag = ((data[5] * 256) + data[4]);
    
  // Output data to serial monitor
  Serial.print("X Axis : ");
  Serial.print(xMag);
  Serial.print(" Y Axis : ");
  Serial.print(yMag);
  Serial.print(" Z Axis : ");
  Serial.println(zMag);
  delay(50);
}
