/**
   @author Yasara JLP
   NRF Sender
*/

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// for GSM
#include <SoftwareSerial.h>
//for magnetometer
#include<Wire.h>

#define soilSensor A0


SoftwareSerial SIM900(10, 11); // object for gsm
RF24 radio(7, 8); // CE, CSN   // object for NRF

//this "incoming_message" and "generatedString" are used to get received msg to GSM.
char incoming_message;
String generatedString = "";
String receivedMsg = "";
String sender;

// for the NRF
const byte address[6] = "00001";

// for magnetometer
#define Addr 0x0E
int prevX , prevY , prevZ , curX, curY, curZ;
bool needInitialize;

// for soil
int soilRead;


// for data
String recipient = "+94769948165";

unsigned long startMillis,currentMillis;
const unsigned long period = 300;


void setup() {
  Serial.begin(19200);

  // --------------------- GSM --------------------- //
  SIM900.begin(19200); // for GSM shield
  delay(1000);  // give time to log on to network.
  configGSM_sms(); // set AT commands related to SMS Receive.

  // --------------------- NRF --------------------- //
  setupNRF();

  // --------------------- Mag --------------------- //
  setupMagneto();

  delay(300);
  startMillis = millis();
}


int x = 3;
void loop() {

  // -------------------------------------------------------------------------------- //
  getAvailableMessage_GSM(); // listening to sms receiver

  if (receivedMsg.length() > 0 && sender.equals(recipient)) { // after received a message
    //Serial.println("cxcx");
    if (receivedMsg.equals("0")) {
      char text[] = "0";
      for (int i = 0; i < 2; i++) {
        radio.write(&text, sizeof(text));
        delay(100);
      }
    } else if (receivedMsg.equals("1")) {
      char text[] = "2";
      for (int i = 0; i < 2; i++) {
        radio.write(&text, sizeof(text));
        delay(100);
      }
    }

    Serial.println("message -> " + receivedMsg);
  }

  // -------------------------------------------------------------------------------- //
  readMagnato();

  soilRead = soilMoistureSensor(soilSensor);

  if (abs(prevX - curX) > 6000 || abs(prevY - curY) > 6000 || abs(prevZ - curZ) > 6000) {
    Serial.println("primary");
    String dataSet = String(curX) + "," + String(curY) + "," + String(curZ) + "," + String(soilRead);
    //sendMessage(recipient, "data");
    Serial.println("emergency send : " + dataSet);
    char text[] = "1";
    for (int i = 0; i < 2; i++) {
      radio.write(&text, sizeof(text));
      delay(100);
    }
    delay(200);
    startMillis = currentMillis;

  } else {
    currentMillis = millis();
    if (currentMillis - startMillis >= 7000) {
      String dataSet = String(curX) + "," + String(curY) + "," + String(curZ) + "," + String(soilRead);
      //sendMessage(recipient, "data");
      delay(200);
      Serial.println("normal send : " + dataSet);
      startMillis = currentMillis;
    }

    

  }

  prevX = curX;
  prevY = curY;
  prevZ = curZ;



}


// for get the current value of soilSensor
int soilMoistureSensor(uint8_t pin) {
  int soilMoistureSensorValue = analogRead(pin);
  //Serial.println(soilMoistureSensorValue);
  return soilMoistureSensorValue;

}


void setupNRF() {
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();

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
// to set the GSM device's state to sms mode & clear all stored sms' in the sim card
void configGSM_sms() {
  SIM900.print("AT+CMGF=1\r");  // set SMS mode to text
  delay(100);
  SIM900.print("AT+CNMI=2,2,0,0,0\r"); // blurt out contents of new SMS upon receipt to the GSM shield's serial out
  delay(100);
}

// to read the incomming message
void getAvailableMessage_GSM() {
  if (SIM900.available() > 0)
  {
    incoming_message = (char)SIM900.read(); //Get the character from the cellular serial port.
    generatedString += String(incoming_message);
    //Serial.print(incoming_message); //Print the incoming character to the terminal.
    receivedMsg = "";
  } else {
    if (generatedString.length() > 50) {
      generatedString.trim();
      sender = generatedString.substring(7, 19); // get the number who sends the message
      receivedMsg = generatedString.substring(48); // get the message
    } else {
      receivedMsg = "";
    }
    //delay(500);
    generatedString = "";
  }
}


// sms sending method
void sendMessage(String phoneNumber, String message) {
  //SIM900.print("AT+CMGF=1\r");
  //delay(100);
  SIM900.println("AT + CMGS = \"" + phoneNumber + "\"");
  delay(100);
  SIM900.println(message);
  delay(100);
  SIM900.println((char)26);
  delay(100);
  SIM900.println();
  delay(200);
}






