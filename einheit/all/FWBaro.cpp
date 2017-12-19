#include "Arduino.h"
#include "FWBaro.h"

Barometer myBarometer;

FWBaro::FWBaro(unsigned long duration):
    Framework(duration) {
}

void FWBaro::init() {
   myBarometer.init();
}

String FWBaro::getData() {
  temperature = myBarometer.bmp085GetTemperature(myBarometer.bmp085ReadUT()); //Get the temperature, bmp085ReadUT MUST be called first
  pressure = myBarometer.bmp085GetPressure(myBarometer.bmp085ReadUP());//Get the temperature
//  altitude = myBarometer.calcAltitude(pressure); //Uncompensated caculation - in Meters
  atm = pressure / 101325;

  String s("\"press\":\"");  //display 2 decimal places
   s.concat(atm);
   s.concat("\",\"temp\":\"");
   s.concat(temperature);
   s.concat("\"");
/*
   s.concat(pressure); // Druck //whole number only.
   s.concat(',');
   s.concat(altitude); // Höhe  //display 2 decimal places
   s.concat(',');
   s.concat(atm);  // Physikalische Atmosphäre  //display 4 decimal places
   */
  return s;
}

const char* FWBaro::getType() {
    return FWBAROTYPE;
}
/*
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
    this->sum += pulseIn(BARO_PIN, LOW);    
  }
}
*/
