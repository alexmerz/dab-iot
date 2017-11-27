#ifndef FWDust_h
#define FWDust_h

#include "Arduino.h"
#include "Framework.h"

#define FWDUSTTYPE "DS"

class FWDust:public Framework {
  public:
    FWDust(unsigned long duration);
    String getData();
    const char* getType();
    void init();
    void check();
  private:
    unsigned long sample_duration;
    unsigned long sampletime_ms;
    unsigned long lowpulseoccupancy;
    float ratio;
    float concentration;
    int samples;
    uint32_t sum;
};

#endif
