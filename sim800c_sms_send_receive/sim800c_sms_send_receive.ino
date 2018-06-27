#include <SoftwareSerial.h>
SoftwareSerial SIM900(10, 11);

//this "incoming_message" and "generatedString" are used to get received msg to this device.
int timeout;
char incoming_message;
String generatedString = "";


// --------------------------- Setup --------------------------- //
void setup()
{
  Serial.begin(19200); // for serial monitor
  // --------------------- GSM --------------------- //
  SIM900.begin(19200); // for GSM shield
  delay(1000);  // give time to log on to network.
  configGSM_sms(); // set AT commands related to SMS Receive.

}

int x = 5;


// --------------------------- Loop ---------------------------- //
void loop()
{
  getAvailableMessage_GSM(); // listening to sms receiver
  
  if(x==5){
    sendMessage("729143275","xxxxx");
    delay(1000);
    sendMessage("729143275","yyyyy");
    x=2;
    }


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

  } else {
    if (generatedString.length() > 50) {
      generatedString.trim();
      Serial.println(generatedString.substring(48));
    }
    //delay(500);
    generatedString = "";
  }
}


// sms sending method
void sendMessage(String phoneNumber, String message) {
  //sim.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  //delay(100);
  //Serial.println ("Set SMS Number");
  SIM900.println("AT+CMGS=\"" + phoneNumber + "\"\r"); //Mobile phone number to send message
  delay(100);
  SIM900.println(message);
  delay(100);
  SIM900.println((char)26);// ASCII code of CTRL+Z
  delay(100);
  readSerial();
}

String readSerial() {
  timeout = 0;
  while  (!SIM900.available() && timeout < 12000  )
  {
    delay(13);
    timeout++;
  }
  if (SIM900.available()) {
    return SIM900.readString();
  }
}
