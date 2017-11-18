#ifndef FWTouch_h
#define FWTouch_h

#include <Arduino.h>
#include "Framework.h"

#define FWTOUCHTYPE "TOUCH"

class FWTouch:public Framework
{
  public:
    FWTouch(unsigned long duration);
    String getData();
    const char* getType();
    void init(int pin);
  private:
    bool state;
    int pin;
};
#endif

