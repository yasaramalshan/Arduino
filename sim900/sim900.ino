#include <SoftwareSerial.h>

// Configure software serial port
SoftwareSerial SIM900(10, 11); 

void setup() {
  Serial.begin(9600);
  SIM900.begin(9600);
  
  delay(2000);   
  Serial.println("setup");
  // Send the SMS
  sendSMS();
}

void loop() { 
  
}

void sendSMS() { Serial.println("in method");
  // AT command to set SIM900 to SMS mode
  SIM900.print("AT+CMGF=1\r"); 
  delay(100);

  // REPLACE THE X's WITH THE RECIPIENT'S MOBILE NUMBER
  // USE INTERNATIONAL FORMAT CODE FOR MOBILE NUMBERS
  SIM900.println("AT + CMGS = \"776721484\""); 
  delay(100);
  
  // REPLACE WITH YOUR OWN SMS MESSAGE CONTENT
  SIM900.println("Message example from Arduino Uno."); 
  delay(100);

  // End AT command with a ^Z, ASCII code 26
  SIM900.println((char)26); 
  delay(100);
  SIM900.println();Serial.println("xxxx");
  // Give module time to send SMS
  delay(5000); 
}
