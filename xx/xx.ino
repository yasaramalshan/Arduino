/*
GPRS Connect TCP

This sketch is used to test seeeduino GPRS_Shield's send SMS func.
to make it work, you should insert SIM card to Seeeduino GPRS 
and replace the phoneNumber,enjoy it!

create on 2015/05/14, version: 1.0
by lawliet.zou(lawliet.zou@gmail.com)
*/
#include <GPRS_Shield_Arduino.h>
#include <SoftwareSerial.h>
#include <Wire.h>

#define PIN_TX    10
#define PIN_RX    11
#define BAUDRATE  19200
#define PHONE_NUMBER "729143275"
#define MESSAGE  "hello,world"

GPRS gprs(PIN_TX,PIN_RX,BAUDRATE);//RX,TX,BaudRate

void setup() {
  Serial.begin(19200);   
  
  
  if(gprs.sendSMS(PHONE_NUMBER,MESSAGE)) //define phone number and text
  {
    Serial.print("Send SMS Succeed!\r\n");
  }
  else {
    Serial.print("Send SMS failed!\r\n");
  }
}

void loop() {
  //nothing to do
}
