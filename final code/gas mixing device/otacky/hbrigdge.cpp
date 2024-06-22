#ifndef hbridge_h
#define hbridge_h

#include "hbridge.h"

void HBridge::begin(int pinA, int pinB, int pinC, int pinD){
  pinMode(pinA, OUTPUT);
  pinMode(pinB, OUTPUT);
  pinMode(pinC, OUTPUT);
  pinMode(pinD, OUTPUT);
  this->pinA = pinA;
  this->pinB = pinB;
  this->pinC = pinC;
  this->pinD = pinD;
}

void HBridge::setDirection(int a, int b){
  if (a < 0){
    digitalWrite(this->pinA, LOW);
    digitalWrite(this->pinB, HIGH);
  }
  else if (a > 0){
    digitalWrite(this->pinA, HIGH);
    digitalWrite(this->pinB, LOW);
  }
  else{
    digitalWrite(this->pinA, LOW);
    digitalWrite(this->pinB, LOW);
  }

  if (b < 0){
    digitalWrite(this->pinC, LOW);
    digitalWrite(this->pinD, HIGH);
  }
  else if (b > 0){
    digitalWrite(this->pinC, HIGH);
    digitalWrite(this->pinD, LOW);
  }
  else{
    digitalWrite(this->pinC, LOW);
    digitalWrite(this->pinD, LOW);
  }
}

#endif
