#include "SPI.h"
#include "Mirf.h"
#include "nRF24L01.h"
#include "MirfHardwareSpiDriver.h"
#include "String.h"

long data;
long tmp2=0;
int led=5;

void setup() {
 Mirf.spi = &MirfHardwareSpi;
 Mirf.init();
 Mirf.setRADDR((byte *)"serv1");
 Mirf.payload = sizeof(long);
 Mirf.config();
 Serial.begin(115200);


}

void loop() {
receive();

}

void receive(){
  if(!Mirf.isSending() && Mirf.dataReady()){
    Mirf.getData((byte *)&data);
    Mirf.rxFifoEmpty();
    Serial.println(data);
    ledBlink(data);
  }
  }
  
void ledBlink(long d){
  if(d==1){
    while(true){
      digitalWrite(led,HIGH);
      delay(2000);
      digitalWrite(led,LOW);
      delay(2000);
      }
    }
    else if(d==2){
    while(true){
      digitalWrite(led,HIGH);
      delay(1000);
      digitalWrite(led,LOW);
      delay(1000);
      }
    }

    else if(d==3){
    while(true){
      digitalWrite(led,HIGH);
      delay(500);
      digitalWrite(led,LOW);
      delay(500);
      }
    }
    else if(d==4){
    while(true){
      digitalWrite(led,HIGH);
      delay(200);
      digitalWrite(led,LOW);
      delay(200);
      }
    }
  
  
  }
















  
void receiveCommand(){
  if(!Mirf.isSending() && Mirf.dataReady()){
    Mirf.getData((byte *)&data);
    Mirf.rxFifoEmpty();
    tmp2=tmp2+data;
    //Serial.println(tmp2);
    Serial.println(data);
    
    if(tmp2>=15){
    Serial.println("data packet received");
    
    tmp2=0;
  }else{
   // Serial.println("No data to be recieved");
  }
}
}


