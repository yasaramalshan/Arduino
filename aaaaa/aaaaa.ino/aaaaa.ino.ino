void setup() {
  // put your setup code here, to run once:
  pinMode(6,OUTPUT);
  digitalWrite(6,HIGH);
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(F("asd"));
}
