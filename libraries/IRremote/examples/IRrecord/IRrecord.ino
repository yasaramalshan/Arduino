#include <IRremote.h>

int receiver = 11;

IRrecv irrecv(receiver);

decode_results results;

void setup()
{
  Serial.begin(9600);
  Serial.println("IR Receiver Button Decode");
  irrecv.enableIRIn(); // Start the receiver

}

void loop() {
  if (irrecv.decode(&results)){
    translateIR();
    Serial.println(results.value);
    irrecv.resume();
    
    }
  
}

void translateIR(){
  switch(results.value){
    case 0xFF629D:
      Serial.println("Forward");
      break;

    case 0xFF22DD:
      Serial.println("Left");
      break;

    case 0xFF02FD:
      Serial.println("Ok");
      break;

    case 0xFFC23D:
      Serial.println("Right");
      break;

    case 0xFFA857:
      Serial.println("Reverse");
      break;

    case 0xFF6897:
      Serial.println("1");
      break;

    case 0xFF9867:
      Serial.println("2");
      break;

    case 0xFFB04F:
      Serial.println("3");
      break;

    case 0xFF30CF:
      Serial.println("4");
      break;

    case 0xFF18E7:
      Serial.println("5");
      break;

    case 0xFF7A85:
      Serial.println("6");
      break;

    case 0xFF10EF:
      Serial.println("7");
      break;

    case 0xFF38C7:
      Serial.println("8");
      break;

    case 0xFF5AA5:
      Serial.println("9");
      break;

    case 0xFF42BD:
      Serial.println("*");
      break;

    case 0xFF4AB5:
      Serial.println("0");
      break;

    case 0xFF52AD:
      Serial.println("#");
      break;

    case 0xFFFFFFFF:
      Serial.println("Repeat");
      break;


    
    }
  
  }








