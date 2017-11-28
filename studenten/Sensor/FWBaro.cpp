#include "Arduino.h"
#include "FWBaro.h"

#define BARO_PIN D2

float temperature;
float pressure;
float atm;
float altitude;
Barometer myBarometer;


FWBaro::FWBaro(unsigned long duration):
    Framework(duration) {
}

void FWBaro::init() {
   myBarometer.init();

  this->samples = 0;
  this->sum = 0;
}

String FWBaro::getData() {
  temperature = myBarometer.bmp085GetTemperature(myBarometer.bmp085ReadUT()); //Get the temperature, bmp085ReadUT MUST be called first
  pressure = myBarometer.bmp085GetPressure(myBarometer.bmp085ReadUP());//Get the temperature
  altitude = myBarometer.calcAltitude(pressure); //Uncompensated caculation - in Meters
  atm = pressure / 101325;

  String s(temperature);
  s.concat(',');
  s.concat(pressure);
  s.concat(',');
  s.concat(altitude);
  s.concat(',');
  s.concat(atm);  
  return s;
}

const char* FWBaro::getType() {
    return FWBAROTYPE;
}

void FWBaro::check() {
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
    this->sum += digitalRead(BARO_PIN);
  }
}
