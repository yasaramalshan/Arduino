/*
  Author     : Dileepa Jayawardena
  how to build Ultrasonic Obstacle Avoidance Robot
  http://myhub.lk/a/How_to_Build/Obstacle-Avoidance-Robot
*/
#include <Servo.h>

Servo servo;
//Motor sheld Pins
//M1
int enA = 5;
int in1 = 3;
int in2 = 4;
//M2
int enB = 6;
int in3 = 7;
int in4 = 8;

int trigPin = 11;    //Trig - green Jumper
int echoPin = 12;    //Echo - yellow Jumper

long duration, cm;




void setup() {
  Serial.begin(9600);
  servo.attach(9);
  servo.write(90);
  delay(300);
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);

}
/*
   mpower(motor number,rotation,speed);

  motor number
  1-1st motor
  2-2nd motor

  rotation
  1-Forword
  0-Stop
  -1- Backword

  speed
  0-255 PWM value

*/

void mpower(int motor, int rotation, int spd) {
  int pwm;
  int pA;
  int pB;
  if (motor == 1) {
    pwm = enA;
    pA = in1;
    pB = in2;
  } else if (motor == 2) {
    pwm = enB;
    pA = in3;
    pB = in4;
  } else {
    return;
  }
  if (rotation == 0) {
    digitalWrite(pA, LOW);
    digitalWrite(pB, LOW);
  } else if (rotation == 1) {
    digitalWrite(pA, HIGH);
    digitalWrite(pB, LOW);
  } else if (rotation == -1) {
    digitalWrite(pA, LOW);
    digitalWrite(pB, HIGH);
  }
  analogWrite(pwm, spd);

}

int speedval = 0;//Speed of car (MAX 40)
int maxspeed = 40;

int normalspeed = 120;// less than 125 (255/2)

int min_dis = 15;

int mode = 0;
//0-run
//1-scan
//2-rotate

int f_max = 0;
int f_max_ang = 0;

void loop() {
  cm = getDistance();

  if (mode == 0) {
    if (cm > min_dis || cm==0) {
      speedval++;
      if (speedval > maxspeed) {
        speedval = maxspeed;
      }
      mpower(1, 1, normalspeed);
      mpower(2, 1, normalspeed);
      delay(100);
    } else {

      mpower(1, -1, normalspeed * 2);
      mpower(2, -1, normalspeed * 2);

      delay(speedval * 4);
      speedval = 0;
      if (cm > 1 || speedval==5) {
        mode = 1;
      }

    }
    Serial.print("CM-");
    Serial.print(cm);
    Serial.print(",Speed-");
    Serial.print(speedval);
    Serial.print(",Mode-");
    Serial.print(mode);
    Serial.println();
  } else if (mode == 1) {
    f_max = 0;
    servo.write(0);
    delay(200);
for (int a 
    = 0; a <= 180; a += 10) {
      servo.write(a);
      delay(20);
      cm = getDistance() ;
      if(a<=60 && a>=120){
        
      if (cm > f_max) {
        f_max = cm;
        f_max_ang = a;
      }
      Serial.print("A=");
      Serial.print(a);
      Serial.print(",D=");
      Serial.println(cm);
        }
    }
    servo.write(90);
    delay(200);
    Serial.print("MAX:");
    Serial.println(f_max);
    mode = 2;
  } else if (mode == 2) {
    Serial.println("Reverse...");
    mpower(1, -1, normalspeed * 2 );
    mpower(2, -1, normalspeed * 2);
    delay(100);
  
      if (f_max_ang < 90) {
        Serial.println("Rotating LEFT");
        mpower(1, -1, normalspeed * 2);
        mpower(2, 1, normalspeed * 2);
        delay((90-f_max_ang) * 2);
      } else {
        Serial.println("Rotating RIGHT");
        mpower(1, 1, normalspeed * 2 );
        mpower(2, -1, normalspeed * 2);
        delay((f_max_ang - 90) * 2);
      }

      mpower(1, 0, 0);
      mpower(2, 0, 0);
     
      mode = 0;
   

  }

  mpower(1, 0, 0);
  mpower(2, 0, 0);

  //  mpower(1, 1, 50);
  //  mpower(2, 1, 100);

}

long getDistance() {
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH, 100000);
  return (duration / 2) / 29.1;
}


