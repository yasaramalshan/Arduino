#include <Servo.h>
#define trig 2
#define echo 4
#define left 6
#define right 7

Servo motor;
long mesure[2];
long cm;
long t;


void setup() {
  // put your setup code here, to run once:
  pinMode(12,OUTPUT);
  digitalWrite(12,HIGH);
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
  if(ultraSonic()<15){
    digitalWrite(left,LOW);
    digitalWrite(right,LOW);  
    servoMotor();
    if(mesure[0]>mesure[1]){
      digitalWrite(left,LOW);
      digitalWrite(right,HIGH);
      delay(100);
      }else{
        digitalWrite(left,HIGH);
        digitalWrite(right,LOW);
        delay(100);
        }
    delay(1000);
    
    }else{
    digitalWrite(left,HIGH);
    digitalWrite(right,HIGH);  
      
      }
  

}















