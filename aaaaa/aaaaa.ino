
unsigned long startMillis;  //some global variables available anywhere in the program
//unsigned long currentMillis;
const unsigned long period = 300;  //the value is a number of milliseconds
const byte ledPin = 13;    //using the built in LED

void setup()
{
  Serial.begin(115200);  //start Serial in case we need to print debugging info
  pinMode(ledPin, OUTPUT);
  startMillis = millis();  //initial start time
}

void loop()
{
  unsigned long currentMillis = millis();  //get the current "time" (actually the number of milliseconds since the program started)
  if (currentMillis - startMillis >= 300)  //test whether the period has elapsed
  {
    digitalWrite(ledPin, !digitalRead(ledPin));  //if so, change the state of the LED.  Uses a neat trick to change the state
    startMillis = currentMillis;  //IMPORTANT to save the start time of the current LED state.
  }
}
