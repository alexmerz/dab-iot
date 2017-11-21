#include "Arduino.h"
#include "FWLight.h"

#define LIGHT_PIN A1

FWLight::FWLight(unsigned long duration):
    Framework(duration) {
}

void FWLight::init() {
    this->samples = 0;
    this->sum = 0;
}

String FWLight::getData() {
    uint32_t avg = 0;

    avg = (this->sum / this->samples);

    String s(avg);
    return s;
}

const char* FWLight::getType() {
    return FWLIGHTTYPE;
}

void FWLight::check() {
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
    this->sum += analogRead(LIGHT_PIN);
  }
}
