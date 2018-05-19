#include <Servo.h>
#include <IRremote.h>
#define IRreceiver 11
#define trig 2
#define echo 4
#define left 6
#define right 5

IRrecv irrecv(IRreceiver);
decode_results results;

Servo motor;
long mesure[2];//for store left side and right side obsercle distances(if any stay)
long cm;
long t;
char command;
boolean remoteMode=false,ultrasonicMode=false;


void setup() {
  // put your setup code here, to run once:
  pinMode(12,OUTPUT);
  pinMode(10,OUTPUT);
  digitalWrite(12,HIGH);//as 5v pin
  digitalWrite(10,LOW);//as 0v pin
  //*****************//
  motor.attach(8);
  irrecv.enableIRIn(); // Start the receiver
  pinMode(2,OUTPUT);
  pinMode(4,INPUT);
  Serial.begin(9600);
  motor.write(90);
}

long ultraSonic(){
  digitalWrite(trig,HIGH);
  delayMicroseconds(10);
  digitalWrite(trig,LOW);
  t = pulseIn(echo,HIGH);
  cm = t/29/2;
  return cm;
  }
  
void servoMotor(){
  motor.write(180);
  mesure[0]=ultraSonic();
  delay(1000);
  motor.write(0);
  mesure[1]=ultraSonic();
  delay(1000);
  motor.write(90);
   
  }

void loop() {
  // put your main code here, to run repeatedly:
  if (irrecv.decode(&results)){
    command = translateIR();
    
    if(command=='O'){
      ultrasonicMode=false;
      analogWrite(left,0);
      analogWrite(right,0);
      }else if(command=='1'){
        ultrasonicMode=true;
      }else if(command=='F'&& !ultrasonicMode){
        analogWrite(left,65);
        analogWrite(right,65);
      }else if(command=='L'&& !ultrasonicMode){
        analogWrite(left,65);
        analogWrite(right,0);
      }else if(command=='R'&& !ultrasonicMode){
        analogWrite(left,0);
        analogWrite(right,65);
      }else if(command=='B' && !ultrasonicMode){
        analogWrite(left,0);
        analogWrite(right,0);
      }
      
    irrecv.resume();
}

    
  if(ultrasonicMode){
      if(ultraSonic()<30){
        analogWrite(left,0);
        analogWrite(right,0);  
        servoMotor();
        if(mesure[0]>mesure[1]){
          analogWrite(left,0);
          analogWrite(right,65);
          delay(10);
          }else{
            analogWrite(left,65);
            analogWrite(right,0);
            delay(10);
            }
        delay(1000);
        
        }else{
        analogWrite(left,75);
        analogWrite(right,75);  
          
          }
  }

}



char translateIR(){
  switch(results.value){
    case 0xFF629D:
      return 'F';//Forward

    case 0xFF22DD:
      return 'L';//Left

    case 0xFF02FD:
      return 'O';//Ok

    case 0xFFC23D:
      return 'R';//Right

    case 0xFFA857:
      return 'B';//Backward

    case 0xFF6897:
      return '1';

    case 0xFF9867:
      return '2';

    case 0xFFB04F:
      return '3';

    case 0xFF30CF:
      return '4';

    case 0xFF18E7:
      return '5';

    case 0xFF7A85:
      return '6';

    case 0xFF10EF:
      return '7';

    case 0xFF38C7:
      return '8';

    case 0xFF5AA5:
      return '9';

    case 0xFF42BD:
      return '*';

    case 0xFF4AB5:
      return '0';

    case 0xFF52AD:
      return '#';

    case 0xFFFFFFFF:
      return 'X';
  }   
}


