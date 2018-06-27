/**
 * @author Yasara JLP
 * NRF Sender
 */

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8); // CE, CSN

const byte address[6] = "00001";

void setup() {
  Serial.begin(19200);
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
}

void loop() {
  const char text[] = "1";
  radio.write(&text, sizeof(text));
  Serial.println("sent");
  delay(100);
}
