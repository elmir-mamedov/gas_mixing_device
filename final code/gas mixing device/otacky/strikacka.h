#include "Arduino.h"
#include "hbridge.h"
#include <Servo.h>

class Strikacka{
  private:
    int pinA, pinB, pinC, pinD;
    int pinLim1, pinLim2;
    int servoPin;
    Servo ventilServo;
    HBridge motor;

    unsigned long inPulse = 600;
    unsigned long outPulse= 2400;
  public:
    void begin(int pinA, int pinB, int pinC, int pinD, int pinLim1, int pinLim2, int servoPin);
    void pump();
    void setInPulse(unsigned long pulse){this->inPulse = pulse;}
    void setOutPulse(unsigned long pulse){this->outPulse = pulse;}
};
