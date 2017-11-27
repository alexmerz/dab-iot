#include "Arduino.h"
#include "FWSound.h"

#define SOUND_PIN A0

FWSound::FWSound(unsigned long duration):
    Framework(duration) {
}

void FWSound::init() {
    this->samples = 0;
    this->sum = 0;
}

String FWSound::getData() {
    uint32_t avg = 0;

    avg = (this->sum / this->samples);
    String s("\"sound\":\"");
    s+= avg;
    s+= "\"";
    return s;
}

const char* FWSound::getType() {
    return FWSOUNDTYPE;
}

void FWSound::check() {
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
    this->sum += analogRead(SOUND_PIN);
  }
}

