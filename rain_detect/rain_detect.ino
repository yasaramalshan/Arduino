//Attach sensor's Analoig Out (AO) pin to A0 pin of the Arduino
#define rainDropsPin A0
void setup() {
  Serial.begin(9600);  
}
void loop(){
  int sensorReading = analogRead(rainDropsPin);
  if (sensorReading < 700){
  Serial.println("Rain is Detected ");
  }
 else{
  Serial.println("Rain is not Detected ");
 }
 Serial.println(sensorReading);
 delay(500);
}
