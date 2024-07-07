#include "Arduino.h"

class HBridge{
  private:
    int pinA, pinB, pinC, pinD;
  public:
    void begin(int pinA, int pinB, int pinC, int pinD);
    void setDirection(int A, int B);
};
