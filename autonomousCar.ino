#include <Servo.h> 

int pinLB=4;     
int pinLF=5;     

int pinRB=6;    
int pinRF=7;    

int MotorLPWM=5;  
int MotorRPWM=6;  

int inputPin = 11;  
int outputPin =10;  

int Fspeedd = 0;     
int Rspeedd = 0;     
int Lspeedd = 0;      
int directionn = 0;    

Servo myservo;        

int delay_time = 250; 

int Fgo = 8;         
int Rgo = 6;         
int Lgo = 4;         
int Bgo = 2;         

void setup()
 {
      Serial.begin(9600);     
      
      pinMode(pinLB,OUTPUT); 
      pinMode(pinLF,OUTPUT); 
      pinMode(pinRB,OUTPUT);  
      pinMode(pinRF,OUTPUT); 
      
      pinMode(MotorLPWM,  OUTPUT);   
      pinMode(MotorRPWM,  OUTPUT);  
      
      pinMode(inputPin, INPUT);    
      pinMode(outputPin, OUTPUT);     
    
      myservo.attach(9);    
 }
 
void advance(int a)     
 {
      digitalWrite(pinRB,HIGH);  
      digitalWrite(pinRF,LOW);   
      analogWrite(MotorRPWM,180);
      
      digitalWrite(pinLB,HIGH);  
      digitalWrite(pinLF,LOW);   
      analogWrite(MotorLPWM,180);
      
      delay(a * 1);     
 }

void right(int b)        
 {
     digitalWrite(pinRB,LOW);   
     digitalWrite(pinRF,HIGH);
     analogWrite(MotorRPWM,250);
     
     digitalWrite(pinLB,LOW);
     digitalWrite(pinLF,LOW);
     
     delay(b * 100);
 }
 
void left(int c)         
 {
     digitalWrite(pinRB,LOW);
     digitalWrite(pinRF,LOW);
     digitalWrite(pinLB,LOW);   
     digitalWrite(pinLF,HIGH);
     analogWrite(MotorLPWM,250);
     
     delay(c * 100);
 }
 
void turnR(int d)        
 {
     digitalWrite(pinRB,HIGH);  
     digitalWrite(pinRF,LOW);
     analogWrite(MotorRPWM,250);
     
     digitalWrite(pinLB,LOW);
     digitalWrite(pinLF,HIGH);  
     analogWrite(MotorLPWM,250);
     
     delay(d * 50);
 }
 
void turnL(int e)        
 {
     digitalWrite(pinRB,LOW);
     digitalWrite(pinRF,HIGH);   
     analogWrite(MotorRPWM,220);
     
     digitalWrite(pinLB,HIGH);   
     digitalWrite(pinLF,LOW);
     analogWrite(MotorLPWM,220);
     
     delay(e * 50);
 }    
void stopp(int f)         
 {
     digitalWrite(pinRB,LOW);
     digitalWrite(pinRF,LOW);
     digitalWrite(pinLB,LOW);
     digitalWrite(pinLF,LOW);
     
     delay(f * 100);
 }
 
void back(int g)          
 {
     digitalWrite(pinRB,LOW);  
     digitalWrite(pinRF,HIGH);
     analogWrite(MotorRPWM,0);
     
     digitalWrite(pinLB,LOW);  
     digitalWrite(pinLF,HIGH);
     analogWrite(MotorLPWM,230);
     
     delay(g * 500);     
 }
    
void detection()        
 {      
      int delay_time = 200;   
      ask_pin_F();            
      
     if(Fspeedd < 10)         
      {
          stopp(1);               
          back(2);                
      }
           
      if(Fspeedd < 20)         
      {
          stopp(1);               
          ask_pin_L();            
          
          delay(delay_time);      
        
          ask_pin_R();              
          delay(delay_time);        
          
        if(Lspeedd > Rspeedd)   
            {
                 directionn = Lgo;      
            }
        
        if(Lspeedd <= Rspeedd)   
            {
                 directionn = Rgo;      
            } 
        
        if (Lspeedd < 15 && Rspeedd < 15)   
            {
                 directionn = Bgo;              
            }          
      }
      
      if(Fspeedd >= 20)              
          {
               directionn = Fgo;            
          }
    }    
    
void ask_pin_F()   
 {
      myservo.write(90);
      
      digitalWrite(outputPin, LOW);   
      delayMicroseconds(2);
      digitalWrite(outputPin, HIGH);  
      delayMicroseconds(10);
      digitalWrite(outputPin, LOW);    
      
      float Fdistance = pulseIn(inputPin, HIGH);  
      
      Fdistance= Fdistance/5.8/10;       
      
      Serial.print("F distance:");      
      Serial.println(Fdistance);        
      
      Fspeedd = Fdistance;              
 }  
 
 void ask_pin_L()    
 {
      myservo.write(5);
      
      delay(delay_time);
      
      digitalWrite(outputPin, LOW);   
      delayMicroseconds(2);
      digitalWrite(outputPin, HIGH);  
      delayMicroseconds(10);
      digitalWrite(outputPin, LOW);    
      
      float Ldistance = pulseIn(inputPin, HIGH); 
      
      Ldistance= Ldistance/5.8/10;       
      
      Serial.print("L distance:");       
      Serial.println(Ldistance);         
      
      Lspeedd = Ldistance;               
 }  
 
void ask_pin_R()    
 {
      myservo.write(177);
      
      delay(delay_time);
      
      digitalWrite(outputPin, LOW);   
      delayMicroseconds(2);
      digitalWrite(outputPin, HIGH);  
      delayMicroseconds(10);
      digitalWrite(outputPin, LOW);    
      
      float Rdistance = pulseIn(inputPin, HIGH);  
      
      Rdistance= Rdistance/5.8/10;       
      
      Serial.print("R distance:");       
      Serial.println(Rdistance);         
      
      Rspeedd = Rdistance;              
 }  
    
void loop()
 {
    myservo.write(90);  
    detection();        
      
   if(directionn == 2)            
   {
       back(8);                    
       turnL(2);                   
       Serial.print(" Reverse ");   
   }
   
   if(directionn == 6)              
   {
       back(1); 
       turnR(6);                   
       Serial.print(" Right ");    
   }
   
   if(directionn == 4)             
   {  
       back(1);      
       turnL(6);                  
       Serial.print(" Left ");       
   }
     
   if(directionn == 8)                
   { 
        advance(1);                  
        
        Serial.print(" Advance ");   
        Serial.print("   ");    
   }
 }