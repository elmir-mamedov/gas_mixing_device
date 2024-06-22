#ifndef strikacka_h
#define strikacka_h

#include "strikacka.h"

void Strikacka::begin(int pinA, int pinB, int pinC, int pinD, int pinLim1, int pinLim2, int servoPin){
  this->motor.begin(pinA, pinB, pinC, pinD);
  this->motor.setDirection(0, 0);
  this->pinLim1 = pinLim1;
  this->pinLim2 = pinLim2;
  pinMode(pinLim1, INPUT_PULLUP);
  pinMode(pinLim2, INPUT_PULLUP);
  this->ventilServo.attach(servoPin);
  this->ventilServo.writeMicroseconds(this->outPulse);
}

void Strikacka::pump(){
  //Serial.println(this->inPulse);
  this->ventilServo.writeMicroseconds(this->inPulse);
  delay(1500);
  motor.setDirection(1,1);
  while(digitalRead(pinLim1)){
    delay(5);
  }  
  motor.setDirection(0, 0);
  //Serial.println(this->outPulse);
  this->ventilServo.writeMicroseconds(this->outPulse);
  delay(1500);
  motor.setDirection(1, -1);
  while(digitalRead(pinLim2)){
    delay(5);
  }
  motor.setDirection(0, 0);
  
}

#endif
