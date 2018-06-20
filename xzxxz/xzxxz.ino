#include <SoftwareSerial.h>

String readSIM900A();
void gsm_init();
void printSerialData();

SoftwareSerial SIM900A(10,11);

void setup()
{
    Serial.begin(19200);
    SIM900A.begin(19200);

    gsm_init();
    delay(1000);
    
}

void loop()
{   
      SIM900A.println("AT+CMGR=1"); 
      delay(1000);
      printSerialData(); 
      
    String buffer = readSIM900A();

    if (buffer.startsWith("\r\n+CMGR: "))
      {
        Serial.println("*** RECEIVED SMS ***");
        
        // Remove first 51 characters
        buffer.remove(0, 64);
        int len = buffer.length();
        // Remove \r\n from tail
        buffer.remove(len - 2, 2);
        
        Serial.println(buffer);
        
        Serial.println("*** END SMS ***");
      }

    delay(100);
}


String readSIM900A()
{
    Serial.println("Hello 2");
    Serial.println("\n");
    
    String buffer;

    while (SIM900A.available())
    {
        //Serial.println("OPEN");
        //Serial.println("\n");
        char c = SIM900A.read();
        buffer.concat(c);
        delay(10);
    }

    Serial.println("Hello 3");
    Serial.println("\n");
    return buffer;
}

void gsm_init()
    {
      SIM900A.println("AT+CPIN?");
      delay(1000);
      printSerialData();

      SIM900A.println("AT+CREG?");
      delay(1000);
      printSerialData();

      SIM900A.println("AT+CNMI=2,2,0,0,0"); 
      delay(1000);
      printSerialData();

      SIM900A.println("AT+CPMS=\"SM\""); 
      delay(1000);
      printSerialData();
    }

void printSerialData()
    {
      while(SIM900A.available()!=0)
      Serial.write(SIM900A.read());
    }
