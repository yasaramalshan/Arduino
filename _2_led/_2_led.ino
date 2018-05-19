int led1 = 13;
int led2 = 12;

void setup() {
  pinMode (led1,OUTPUT);
  pinMode (led2,OUTPUT);
}

void loop() {
  digitalWrite (led1,HIGH);
  delay(100);
  digitalWrite (led1,LOW);
  delay(100);
  digitalWrite (led1,HIGH);
  delay(100);
  digitalWrite (led1,LOW);
  delay(100);
  digitalWrite (led2,HIGH);
  delay(100);
  digitalWrite (led2,LOW);
  delay(100);
  digitalWrite (led2,HIGH);
  delay(100);
  digitalWrite (led2,LOW);
  delay(100);
  digitalWrite (led2,HIGH);
  delay(100);
  digitalWrite (led2,LOW);
  delay(100);
  

}
