/**
 * @author Yasara JLP
 * Maze Runner
 * 
 */
    
    #define motor_left 10  // controls left motor speed
    #define motor_right 11  // controls left motor speed         
    #define L1 8  // controls the direction of rotation of the left motor
    #define L2 9  // controls the direction of rotation of the left motor
    #define R1 12 // controls the direction of rotation of the right motor
    #define R2 7  // controls the direction of rotation of the right motor
 
    int trigger_front = A4; // controls the pulse sent from the front sensor
    int echo_front = A5; // controls the pulse received from the front sensor
    
    int trigger_left = A2;// controls the pulse sent from the left sensor
    int echo_left = A3;// controls the pulse received from the left sensor
    
    int trigger_right = A0;// controls the pulse sent from the right sensor
    int echo_right = A1;// controls the pulse received from the right sensor
    
        
    void setup()
    {
        pinMode(trigger_front, OUTPUT); // set trigger_front as a signal output
        pinMode(echo_front, INPUT);// set echo_front as a signal input
        
        pinMode(trigger_left, OUTPUT);// set trigger_left as a signal output
        pinMode(echo_left, INPUT);// set echo_left as a signal input        
        
        pinMode(trigger_right, OUTPUT);// set trigger_right as a signal output
        pinMode(echo_right, INPUT);// set echo_right as a signal input
       
        pinMode(motor_left, OUTPUT);// set motor_left as an output
        pinMode(motor_right, OUTPUT);// set motor_right as an output
     
        pinMode(L1, OUTPUT);
        pinMode(L2, OUTPUT);
        pinMode(R1, OUTPUT);
        pinMode(R2, OUTPUT);       
        delay(5000);
    }
    

    
    void loop()
    {
     // declaration of variables used to control the project
     long duration_front, duration_left, duration_right, right, left, front; 
     
     
     digitalWrite(trigger_front, LOW);  
     delayMicroseconds(2);              
     digitalWrite(trigger_front, HIGH); 
     delayMicroseconds(5);               
     digitalWrite(trigger_front, LOW); 
     duration_front = pulseIn(echo_front, HIGH);
     front = duration_front/29/2; // this is the distance to front obstacle in cm
    
     digitalWrite(trigger_left, LOW);
     delayMicroseconds(2);
     digitalWrite(trigger_left, HIGH);
     delayMicroseconds(5);
     digitalWrite(trigger_left, LOW); 
     duration_left = pulseIn(echo_left, HIGH);
     left = duration_left/29/2;
    
     digitalWrite(trigger_right, LOW);
     delayMicroseconds(2);
     digitalWrite(trigger_right, HIGH);
     delayMicroseconds(5);
     digitalWrite(trigger_right, LOW); 
     duration_right = pulseIn(echo_right, HIGH);
     right = duration_right/29/2; 
     
     analogWrite(motor_left, 0);  
     analogWrite(motor_right, 0); 
     analogWrite(L1, 0);            
     analogWrite(L2, 0);            
     analogWrite(R1, 0);            
     analogWrite(R2, 0);         
     
       if(front >8) // if in case there is free path to front it follows this logic below
         {  
           // below if statements are for the control of robot drivability,
           // in order to keep it following the right wall straight;  
           
           if(right >7 && right< 13){ // if the distance from the wall to the right is between 9 and 12 cm, the robot remains in a straight line;
                            
               analogWrite(motor_left, 120);
               analogWrite(motor_right, 150);
           
               analogWrite(L1, 255);
               analogWrite(L2, 0);
               analogWrite(R1, 0);
               analogWrite(R2, 255);                                                        
             }
         
           if(right >=13){  // if the distance from the wall to the right is greater than or equal to 13 cm, the robot increases its left motor speed to approach the right wall;
             
               analogWrite(motor_left, 255);
               analogWrite(motor_right, 60);
           
               analogWrite(L1, 255);
               analogWrite(L2, 0);
               analogWrite(R1, 0);
               analogWrite(R2, 255);                               
             }
             
                 
           if(right <=7){  // if the distance from the wall to the right is less than or equal to 8 cm, the robot increases its right motor speed to move away from the wall right;
             
               analogWrite(motor_left, 60);
               analogWrite(motor_right, 255);
           
               analogWrite(L1, 255);
               analogWrite(L2, 0);
               analogWrite(R1, 0);
               analogWrite(R2, 255);                   
             }
         }
         
              
       if(left <=20 && right>20 && front <=8) 
          rotate_right();  
            
       if(left >20 && right>20 && front <=8) 
          rotate_right(); 
       
       if(right <=20 && left>20 && front <=8) 
          rotate_left();
       
       if(right<=20 && left<=20 && front<=8) 
          rotate_back();                              
    
    }
       
    void rotate_left() // function to make the robot rotate 90º to left if it does not have front and right;
      {
       
        analogWrite(motor_left, 120);
        analogWrite(motor_right, 120);
           
        analogWrite(L1, 0);
        analogWrite(L2, 255);
        analogWrite(R1, 0);
        analogWrite(R2, 255);
        delay(700);      
        
      }
    
    void rotate_right() // function to make the robot rotate 90º to right if it does not have front or left
      {
        
      
        analogWrite(motor_left, 120);
        analogWrite(motor_right, 120);
        
        analogWrite(L1, 255);
        analogWrite(L2, 0);
        analogWrite(R1, 255);
        analogWrite(R2, 0);             
        delay(800);
               
      }
    
    void rotate_back() // function to make the robot rotate 180º if it does not have front, right and left
      {
       
        analogWrite(motor_left, 120);
        analogWrite(motor_right, 120);
           
        analogWrite(L1, 255);
        analogWrite(L2, 0);
        analogWrite(R1, 255);
        analogWrite(R2, 0);
        delay(1200); 
         
      }   
  
    
    
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            
