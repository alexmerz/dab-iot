#ifndef FWDUST2_h
#define FWDUST2_h

#include "Framework.h"
#include "Arduino.h"
#include "vmdcl.h"
#include "vmdcl_gpio.h"
#include "vmdcl_eint.h"
#include "WInterrupts.h"

#define FWDUST2TYPE "FWDUST2"


class FWDust2:public Framework
{
  public:
    FWDust2();
    void check();
    String getData();
    const char* getType();
    void init();
    void enableEINT(int pin);
    void switchEINT(int pin, int state);
  protected:
    float concentration;
    float ratio;    
    unsigned long wait_before_measurement;
    unsigned long lowpulseoccupancy;
    unsigned long eint_starttime;
    unsigned long sensor_starttime;
    bool old_state;
    unsigned long sampletime_ms;
};

#endif
