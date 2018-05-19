#include <Servo.h>
#define trig 2
#define echo 4
#define left 6
#define right 5

Servo motor;
long mesure[2];//for store left side and right side obsercle distances(if any stay)
long cm;
long t;
boolean remoteMode=false,ultrasonicMode=false;


void setup() {
  // put your setup code here, to run once:
  pinMode(12,OUTPUT);
  pinMode(10,OUTPUT);
  digitalWrite(12,HIGH);//as 5v pin
  digitalWrite(10,LOW);//as 0v pin
  //*****************//
  motor.attach(8);
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











