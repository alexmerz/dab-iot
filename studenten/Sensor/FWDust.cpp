#include "Arduino.h"
#include "FWDust.h"

#define DUST_PIN 8

FWDust::FWDust(unsigned long duration):
    Framework(duration) {
}

void FWDust::init() {
  sampletime_ms = 30000;
  lowpulseoccupancy = 0;
  ratio = 0;
  concentration = 0;
  
  this->samples = 0;
  this->sum = 0;
}

String FWDust::getData() {
  ratio = (sum/(sampletime_ms*10.0));  // Integer percentage 0=&gt;100
  concentration = 1.1*pow(ratio,3)-3.8*pow(ratio,2)+520*ratio+0.62; // using spec sheet curve

  String s(concentration);
  return s;
}

const char* FWDust::getType() {
  return FWDUSTTYPE;
}

void FWDust::check() {
  if(millis() > _nextts) {
    _nextts = millis() + _duration;  

    if(_callback) {
      _callback(*this);
      
      this->samples = 0;
      this->sum = 0;
    }
  } 
  else {
    this->samples++;
    this->sum += pulseIn(DUST_PIN, LOW);
  }
}

