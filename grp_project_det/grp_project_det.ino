/**
   @author Yasara JLP
   NRF Sender
*/

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// for GSM
#include <SoftwareSerial.h>

#define soilSensor A0


SoftwareSerial SIM900(10, 11); // object for gsm
RF24 radio(7, 8); // CE, CSN   // object for NRF

//this "incoming_message" and "generatedString" are used to get received msg to GSM.
char incoming_message;
String generatedString = "";
String receivedMsg = "";

// for the NRF
const byte address[6] = "00001";

void setup() {
  Serial.begin(19200);

  // --------------------- GSM --------------------- //
  SIM900.begin(19200); // for GSM shield
  delay(1000);  // give time to log on to network.
  configGSM_sms(); // set AT commands related to SMS Receive.

  // --------------------- NRF --------------------- //
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
}


int x = 3;
void loop() {
  //char text[] = "data sent....!";
  //radio.write(&text, sizeof(text));// transmit nrf message
  //Serial.println("sent");
  //Serial.print("Soil Sensor Reading : ");
  //Serial.println(soilMoistureSensor(soilSensor));
  //delay(1000);
  getAvailableMessage_GSM(); // listening to sms receiver

  if (receivedMsg.length() > 0) { // after received a message
    if (receivedMsg.equals("0")) {
      char text[] = "0";
      for(int i = 0;i<2;i++){
        radio.write(&text, sizeof(text));
        delay(100);
        }
    } else if (receivedMsg.equals("1")) {
      char text[] = "2";
      for(int i = 0;i<2;i++){
        radio.write(&text, sizeof(text));
        delay(100);
        }
    }

    Serial.println("message -> " + receivedMsg);
  }

  if (x == 5) {
    sendMessage("776721484", "asd5asd"); x = 2;
  }
}


// for get the current value of soilSensor
int soilMoistureSensor(uint8_t pin) {
  int soilMoistureSensorValue = analogRead(pin);
  Serial.println(soilMoistureSensorValue);
  delay(100);
  return soilMoistureSensorValue;

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
      receivedMsg = generatedString.substring(48);
    } else {
      receivedMsg = "";
    }
    delay(500);
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
  delay(2000);
}






