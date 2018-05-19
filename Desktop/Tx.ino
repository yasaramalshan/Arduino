#include <MirfHardwareSpiDriver.h>
#include <MirfSpiDriver.h>
#include <Mirf.h>
#include <nRF24L01.h>
#include <SPI.h>
#include <Wire.h>


long command;
long last_command;


void setup() {
  
  Mirf.spi = &MirfHardwareSpi;  
  Mirf.init(); 
  Mirf.setTADDR((byte *)"serv1");
  Mirf.payload = sizeof(long);
  Mirf.config(); 
  Serial.begin(115200);
}

void loop() {
  //dataS(2);
  //dataS(3);
  dataS(4);
  delay(100);



  }

void dataS(long cmd){
    command=cmd;
    delay(500);
    Mirf.send((byte *)&command);
    while(Mirf.isSending()){
    }
    Serial.println("data packet sent");
  }





















  
void dataSend(){
  
  for(int i=1;i<=5;i++){
    command =i;
    delay(500);
    Mirf.send((byte *)&command);
    
    while(Mirf.isSending()){
    }
    
  }
  Serial.println("data packet sent");
  delay(1000);

  
  }
