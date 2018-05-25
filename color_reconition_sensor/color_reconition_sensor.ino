/**
 * @author Yasara JLP
 * Color Recognition Sensor
 */
 
int s0 = 4;
int s1 = 5;
int s2 = 6;
int s3 = 7;
int sensorOut = 8;

int frequency = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(s0,OUTPUT);
  pinMode(s1,OUTPUT);
  pinMode(s2,OUTPUT);
  pinMode(s3,OUTPUT);
  pinMode(sensorOut,INPUT);

  // setting frequency scaling to 20%
  digitalWrite(s0,HIGH);
  digitalWrite(s1,LOW);

  Serial.begin(9600);
  
  
}

void loop() {
  // setting red filter photodiode to be read
  digitalWrite(s2,LOW);
  digitalWrite(s3,LOW);
  //read the output frequency
  frequency = pulseIn(sensorOut, LOW);
  // remap frequency
  frequency = map(frequency,20,70,255,0);
  //print
  Serial.print("Red = ");
  Serial.print(frequency);
  Serial.print("  ");
  delay(100);

  // setting green filter photodiode to be read
  digitalWrite(s2,HIGH);
  digitalWrite(s3,HIGH);
  //read the output frequency
  frequency = pulseIn(sensorOut, LOW);
  // remap frequency
  frequency = map(frequency,20,70,255,0);
  //print
  Serial.print("Green = ");
  Serial.print(frequency);
  Serial.print("  ");
  delay(100);

  // setting blue filter photodiode to be read
  digitalWrite(s2,LOW);
  digitalWrite(s3,HIGH);
  //read the output frequency
  frequency = pulseIn(sensorOut, LOW);
  // remap frequency
  frequency = map(frequency,20,70,255,0);
  //print
  Serial.print("Blue = ");
  Serial.print(frequency);
  Serial.print("\n");
  delay(100);

}









