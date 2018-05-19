void setup() {
  for(int x=13;x>=8;x--){
    pinMode(x,OUTPUT);  
  } 

}

void loop() {
   for(int x=8;x<=13;x++){
    digitalWrite(x,HIGH);
    delay(80);
    digitalWrite(x,LOW);
    delay(80);  
    }

  for (int y=13;y>=8;y--){
    digitalWrite(y,HIGH);
    delay(80);
    digitalWrite(y,LOW);
    delay(80); 
    }
 

  
 

}
