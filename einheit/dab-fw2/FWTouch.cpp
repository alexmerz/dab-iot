#ifndef FWTouch_cpp
#define FWTouch_cpp

#include <Arduino.h>
#include "FWTouch.h"

FWTouch::FWTouch(unsigned long duration):
Framework(duration)
{
  state = 0;
}

void FWTouch::init(int pin)
{  
  pinMode(pin, INPUT);  
  this->pin = pin;  
}

String FWTouch::getData()
{
    return String(digitalRead(pin));  
}

const char* FWTouch::getType()
{
  return FWTOUCHTYPE;
}
#endif

