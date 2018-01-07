#ifndef FWHumi_h
#define FWHumi_h

#include "Arduino.h"
#include "Framework.h"
#include "DHT.h"

#define HUMIPIN 2

#define FWHUMITYPE "HUMI"

class FWHumi:public Framework {
  public:
      FWHumi(unsigned long duration);
      String getData();
      const char* getType();
      void init();
  private:
      float temperature;
      float humidity;
};

#endif
