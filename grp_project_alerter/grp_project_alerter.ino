/**
   @author Yasara JLP
   NRF Receiver
*/

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8); // CE, CSN

const byte address[6] = "00001";

#define buzzer 3

String state;

void setup() {
  // put your setup code here, to run once:
  pinMode(buzzer, OUTPUT);
  Serial.begin(19200);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
}

void loop() {
  // put your main code here, to run repeatedly:

  if (radio.available()) {
    char text[32] = "";
    radio.read(&text, sizeof(text));
    state = text;
    Serial.println(text);

  }

   if (state.equals("0")) {
      digitalWrite(buzzer,LOW);
    } else if (state.equals("1")) {
      primaryAlert();
    } else if (state.equals("2")) {
      secondaryAlert();
    }
  
}

void primaryAlert() {
  digitalWrite(buzzer, HIGH);
  delay(100);
  digitalWrite(buzzer, LOW);
  delay(1500);
}

void secondaryAlert() {
  digitalWrite(buzzer, HIGH);
  delay(100);
  digitalWrite(buzzer, LOW);
  delay(100);
}
