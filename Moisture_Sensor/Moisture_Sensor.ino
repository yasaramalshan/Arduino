

int sensorPin = A0;  
int sensorValue = 0;  
int percent = 0;

void setup() {
  Serial.begin(9600);
}

void loop() {
  
  sensorValue = analogRead(sensorPin);
  percent = convertToPercent(sensorValue);
  printValuesToSerial();
  delay(500);
}

int convertToPercent(int value)
{
  int percentValue = 0;
  percentValue = map(value, 1023, 340, 0, 100); // ----------------------------------mapping----------------------------------
  return percentValue;
}

void printValuesToSerial()
{
  Serial.print("\n\nAnalog Value: ");
  Serial.print(sensorValue);
  Serial.print("\nPercent: ");
  Serial.print(percent);
  Serial.print("%");
}

