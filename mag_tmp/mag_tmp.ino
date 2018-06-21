/**
   @author Yasara JLP
   MAG3110 magnetometer - I2C

*/


#include<Wire.h>

int prevX , prevY , prevZ , curX, curY, curZ;
bool needInitialize;

// MAG3110 I2C address is 0x0E(14)
#define Addr 0x0E

void setup() {
  Serial.begin(9600);
  setupMagneto();

  delay(300);
}


void loop() {
  readMagnato();

  if (abs(prevX - curX) > 5500 || abs(prevY - curY) > 5500 || abs(prevZ - curZ) > 5500) {
    Serial.println("primary");
    delay(400);

  }else{
    
    }

  prevX = curX;
  prevY = curY;
  prevZ = curZ;

}



void setupMagneto() {
  needInitialize  = true; // to set prev variables
  Wire.begin();// Initialise I2C communication as MASTER
  Wire.beginTransmission(Addr);// Start I2C Transmission
  Wire.write(0x10);// Select control register-1
  Wire.write(0x01);// Set active mode enabled
  Wire.endTransmission();// Stop I2C Transmission
  readMagnato(); // set prev X,Y,Z values
}


void readMagnato() {
  unsigned int data[6];
  Wire.beginTransmission(Addr); // Start I2C Transmission
  Wire.write(0x01); // Select data register
  Wire.endTransmission(); // Stop I2C Transmission
  Wire.requestFrom(Addr, 6); // Request 6 bytes of data

  // Read 6 bytes of data
  // xMag lsb, xMag msb, yMag lsb, y Mag msb, zMag lsb, zMag msb
  if (Wire.available() == 6) {
    data[0] = Wire.read();
    data[1] = Wire.read();
    data[2] = Wire.read();
    data[3] = Wire.read();
    data[4] = Wire.read();
    data[5] = Wire.read();
  }

  if (needInitialize) {
    prevX = ((data[1] * 256) + data[0]);
    prevY = ((data[3] * 256) + data[2]);
    prevZ = ((data[5] * 256) + data[4]);
    needInitialize = false;
  } else {
    curX = ((data[1] * 256) + data[0]);
    curY = ((data[3] * 256) + data[2]);
    curZ = ((data[5] * 256) + data[4]);
  }

  delay(50);
}
