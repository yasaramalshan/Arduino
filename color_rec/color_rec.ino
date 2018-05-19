#include <LiquidCrystal.h>
#include <FreqCount.h>

int s0 = 8;
int s1 = 9;
int s2 = 12;
int s3 = 11;
int out = 10;
int led = 13;
int unChar;
unsigned long absolute,red,green,blue;

LiquidCrystal lcd(7,6,5,4,3,2);//LCD pins


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  FreqCount.begin(10);
  lcd.begin(16, 2);

  pinMode(led,OUTPUT);
  //color mod selection
  pinMode(s2,OUTPUT);
  pinMode(s3,OUTPUT);

  pinMode(out,INPUT);
  //comunication freq (sensitivity) selection
  pinMode(s0,OUTPUT);
  pinMode(s1,OUTPUT);
  
  
}

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available()>=0){
    unChar = Serial.read();
    if (unChar == 'R'){
      digitalWrite(led,HIGH);
      digitalWrite(s0,HIGH);
      digitalWrite(s1,HIGH);
      digitalWrite(s2,LOW);
      digitalWrite(s3,LOW);
      delay(250);
      lcd.clear();
      absolute = (FreqCount.read()/10)*2;
      red = absolute;
      if(absolute > 255){
        lcd.setCursor(0,0);
        lcd.print("Red:");
        lcd.print(255);
        Serial.print(255);
        
        }else{
          lcd.setCursor(0,0);
          lcd.print("Red:");
          lcd.print(red);
          Serial.print(red);
         }
      
    }
    else if (unChar == 'G') {
      digitalWrite(led,HIGH);
      digitalWrite(s0,HIGH);
      digitalWrite(s1,HIGH);
      digitalWrite(s2,HIGH); // green filter select
      digitalWrite(s3,HIGH); // green filter select
      delay(250);
      absolute = (FreqCount.read()/10)*2;
      green = absolute;
      if(absolute > 255){
        lcd.setCursor(0,1);
        lcd.print("Green:");
        lcd.print(255);
        Serial.print(255);
        
        }else{
          lcd.setCursor(0,1);
          lcd.print("Green:");
          lcd.print(green);
          Serial.print(green);
         }
    }
    else if (unChar == 'B') {
      digitalWrite(led,HIGH);
      digitalWrite(s0,HIGH);
      digitalWrite(s1,HIGH);
      digitalWrite(s2,LOW); // blue filter select
      digitalWrite(s3,HIGH); // blue filter select
      delay(250);
      absolute = (FreqCount.read()/10)*2;
      blue = absolute;
      if(absolute > 255){
        lcd.setCursor(0,0);
        lcd.print("Blue:");
        lcd.print(255);
        Serial.print(255);
        
        }else{
          lcd.setCursor(0,0);
          lcd.print("Blue:");
          lcd.print(blue);
          Serial.print(blue);
         }
    }
    
    }
}





