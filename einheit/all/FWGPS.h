#ifndef FWGPS_h
#define FWGPS_h

#include "Arduino.h"

#include "Framework.h"

#define FWGPSTYPE "GPS"
#define FWGPS_VALID 1
#define FWGPS_ALTITUDE 2
#define FWGPS_LONGITUDE 3
#define FWGPS_LATITUDE 4
#define FWGPS_TIME 5
#define FWGPS_DATE 6

class FWGPS:public Framework
{
  public:
    FWGPS(unsigned long duration);
    void check();
    String getData();
    const char* getType();
    void init();
    void on();
    void off();
    int isValid();
  protected:  
    String time;
    String date;
    String latitude;
    String longitude;
    String altitude;
    String valid;
    String gprmc;
    String gpgga;
    String result;
    unsigned long _duration;
    unsigned long _nextts;
};

#endif
