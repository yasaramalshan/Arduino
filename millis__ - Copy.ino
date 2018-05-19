int state = LOW;
unsigned long preMillis=0;
unsigned long start=0;
void setup() {
  // put your setup code here, to run once:
  //Serial.begin(9600);
  pinMode(12,OUTPUT);
  pinMode(10,OUTPUT);
}

void loop() {
  digitalWrite(10,HIGH);
  myDelay(2000);
  digitalWrite(10,LOW);
  myDelay(2000);
  
  }

void myDelay(int duration){
  start = millis();
  while(millis()-preMillis <= duration){
    ledBlink(200);
    }
    
  }

void ledBlink(int blinkDuration){
  if (millis()-start > blinkDuration){
    if (state == LOW){
      state = HIGH;
      }else{
        state = LOW;
        }
    digitalWrite(12,state);
    start = millis();
    }
  
  }

