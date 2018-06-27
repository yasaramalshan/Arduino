#include <SoftwareSerial.h>
SoftwareSerial sim(10, 11);
int timeout;
String number = "+94729143275";
String message;

void setup() {
  
  Serial.begin(19200);
  Serial.println("Sistem Started...");
  sim.begin(19200);
  message="zbc";
  SendMessage();
  delay(1000);
  message="mjgty";
  SendMessage();
}
void loop() {
  
}
void SendMessage()
{
  //Serial.println ("Sending Message");
  sim.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(100);
  //Serial.println ("Set SMS Number");
  sim.println("AT+CMGS=\"" + number + "\"\r"); //Mobile phone number to send message
  delay(100);
  sim.println(message);
  delay(100);
  sim.println((char)26);// ASCII code of CTRL+Z
  delay(100);
  readSerial();
}

String readSerial() {
  timeout = 0;
  while  (!sim.available() && timeout < 12000  )
  {
    delay(13);
    timeout++;
  }
  if (sim.available()) {
    return sim.readString();
  }
}
