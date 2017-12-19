#ifndef FWBaro_h
#define FWBaro_h

#include "Arduino.h"
#include "Framework.h"
#include "Barometer.h"
#include <Wire.h>

#define FWBAROTYPE "BARO"

class FWBaro:public Framework {
  public:
      FWBaro(unsigned long duration);
      String getData();
      const char* getType();
      void init();
      void check();
  private:
      float temperature;
      float pressure;
      float atm;
      float altitude;
      int samples;
      uint32_t sum;
      Barometer myBarometer;

};

#endif
