/*
 * loop:
1.move to max. pos.
2.from max. pos. move 10 spins down.
3.wait for monitor input to continue
4.move another 10 spins down.
5. steps 3-4 until you're went down 120 spins.
6. wait for monitor input
7. move to minimum state.
8. wait for user input.
*/


#include <Servo.h>
#include "strikacka.h"

Strikacka strik;

Servo servo1;
Servo servo2;

void setup() {

  servo1.attach(9); // attach servo to pin 9
  delay(1000);
  servo2.attach(10); // attach servo to pin 10
  
  Serial.begin(9600);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(7, INPUT_PULLUP);
  pinMode(8, INPUT_PULLUP);
  digitalWrite(2, LOW);
  digitalWrite(3, LOW);


  
}

void loop() {
  while(Serial.available()){ //clear buffer
  Serial.read();
  }
  

  
  int otacka;
  int userInput = Serial.parseInt();           // do pocatecni polohy
  int maximumSensorState = digitalRead(7);     
  int minimumSensorState = digitalRead(8);     
  int celk_otacky = 0;
  otacka = analogRead(A0);
  int iterace=1;

  Serial.println("do max. polohy");
  
  servo1_zavreno();
  servo_ref();
  
  in();
  
  servo_odpad();
  servo2_zavreno();
  
  while(iterace<13){ 
    while(!Serial.available()){
      delay(10);
    }
    
    Serial.print("iterace ");
    Serial.println(iterace);
    while (celk_otacky < iterace*10) {
      otacka = analogRead(A0);
      maximumSensorState = digitalRead(7);     // senzor u motoru - stisknuty = 0
      minimumSensorState = digitalRead(8);     // senzor u strikacky - stisknuty = 0
    
      digitalWrite(2, LOW);
      digitalWrite(3, HIGH);  
    
      // spin tracker
      if (otacka > 1000) {
        celk_otacky++;
        Serial.print("Pocet otacek: ");
        Serial.println(celk_otacky);
        delay(150);
      } 
    }
    digitalWrite(2, LOW);
    digitalWrite(3, LOW);  
    iterace++;
   
    while(Serial.available()){ //clear buffer
    Serial.read();
    }

  }
  
  while(!Serial.available()){
    delay(10);
  }
  
  Serial.println("dokonceni vyprazdnovani");
  out();
   
  while(Serial.available()){ //clear buffer
  Serial.read();
  }
    
  while(!Serial.available()){
  delay(10);
  }
}

  
void out(){
  int maximumSensorState = digitalRead(7);     
  int minimumSensorState = digitalRead(8);  
  while (minimumSensorState == 1) {
    maximumSensorState = digitalRead(7);     // senzor u motoru - stisknuty = 0
    minimumSensorState = digitalRead(8);     // senzor u strikacky - stisknuty = 0

    digitalWrite(2, LOW);
    digitalWrite(3, HIGH); 
  }
  
  digitalWrite(2, LOW);
  digitalWrite(3, LOW);
  delay(500);
}
  
void in(){
  int maximumSensorState = digitalRead(7);     
  int minimumSensorState = digitalRead(8); 
  while (maximumSensorState == 1) {  
    maximumSensorState = digitalRead(7);     // senzor u motoru - stisknuty = 0
    minimumSensorState = digitalRead(8);     // senzor u strikacky - stisknuty = 0
    digitalWrite(2, HIGH);
    digitalWrite(3, LOW); 
  }
  
  digitalWrite(2, LOW);
  digitalWrite(3, LOW);
  delay(500);

}


 void servo_odpad(){
  servo1.writeMicroseconds(2400);
  delay(500);
}

void servo2_zavreno(){
  servo2.writeMicroseconds(1925);
  delay(500);
}

void servo1_zavreno(){
  servo1.writeMicroseconds(1950);
  delay(500);
}

void servo_ref(){
  servo2.writeMicroseconds(2350);
  delay(500);
}
