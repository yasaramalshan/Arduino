#include <virtuabotixRTC.h> // for the RTC
#include <dht.h>

/* pin declaration of used modules */
#define soilSensor_1 A0
#define soilSensor_2 A1
#define humiditySensor_1 A4
#define raindropSensor_1 A5
//----------------------------------//
#define relayPin_1 12

//----------------------------------//
#define clk 6
#define dat 7
#define rst 8

/* --------------------- Prototypes --------------------- */
// uint8_t used as the data type for a pin number //
void valve(uint8_t pin);
int soilMoistureSensor(uint8_t pin);
int rainDropSensor(uint8_t pin);
float* humiditySensor(uint8_t pin);
int* getDateTime();


/* ----------------------- Objets ----------------------- */
virtuabotixRTC rtc_module(clk, dat, rst); // an Object for RTC Module
dht DHT; // an Object for Humidity Sensor Module

/* -------------------- Arduino Base -------------------- */
void setup(){
  pinMode(relayPin_1,OUTPUT);
  Serial.begin(9600);
}

void loop(){
  //int* dateTime = getDateTime();
  //Serial.print("xxxx---> ");
  //Serial.println(dateTime[0]);

  float* humidTemp = humiditySensor(humiditySensor_1);
  Serial.print("hhhh---> ");
  Serial.print(humidTemp[1]);
  
  if(500<soilMoistureSensor(soilSensor_1)){
    if(rainDropSensor(raindropSensor_1)<450){
      Serial.println("Wahinoooo");
    }else{
      valve(relayPin_1);
    }
  }
  else{
    Serial.println("Wathura thiyanooo");
  }
  
}



/* ---------------------- Function Declarations ---------------------- */

void valve(uint8_t pin){
  Serial.println("Valve open");
  digitalWrite(pin,LOW);
  delay(3000);
  
  Serial.println("Valve close");
  digitalWrite(pin,HIGH);
  delay(3000);
}


int soilMoistureSensor(uint8_t pin){
  int soilMoistureSensorValue=analogRead(pin);
  Serial.println(soilMoistureSensorValue);
  delay(100);
  return soilMoistureSensorValue;
  
}

float* humiditySensor(uint8_t pin){
  static float values[2]; // format : humidity(percentage value)|temperature(celcius)
  DHT.read11(pin);
  values[0] = DHT.humidity;
  values[1] = DHT.temperature;
  return values;
  
}


int rainDropSensor(uint8_t pin){
  int rainDropSensorValue=analogRead(pin);
  Serial.println(rainDropSensorValue);
  delay(100);
  return rainDropSensorValue;
  
}

int* getDateTime(){
  static int dateTime[6];// format : year|month|day|hour|min|sec
  rtc_module.updateTime();
  dateTime[0] = rtc_module.year;
  dateTime[1] = rtc_module.month;
  dateTime[2] = rtc_module.dayofmonth;
  dateTime[3] = rtc_module.hours;
  dateTime[4] = rtc_module.minutes;
  dateTime[5] = rtc_module.seconds;
  
  return dateTime;
}

