/**
 * Implementierung der Sensorklasse für den
 * Feinstaubsensor
 */

#ifndef FWDust_cpp
#define FWDust_cpp

#include "Arduino.h"
#include "FWDust.h"

#define DUST_PIN 8

// Unser Konstruktor ruft den Konstruktor
// der Basisklasse auf und den Konstruktor des ppd-Instanz unseres Objekts
FWDust::FWDust(unsigned long duration):
Framework(duration)//,
//ppd()
{
}

void FWDust::init()
{
  // Speicher für Variablen wurden reserviert in FWDust.h class FWDust:public Framework {}
  sampletime_ms = 30000;
  lowpulseoccupancy = 0;
  ratio = 0;
  concentration = 0;
  
  this->samples = 0;
  this->sum = 0;
}

String FWDust::getData()
{

  // Die Werte auslesen
  ratio = (sum/(sampletime_ms*10.0));  // Integer percentage 0=&gt;100
  concentration = 1.1*pow(ratio,3)-3.8*pow(ratio,2)+520*ratio+0.62; // using spec sheet curve

  String s(concentration);
  return s;
}

const char* FWDust::getType()
{
  return FWDUSTTYPE;
}

void FWDust::check() {
  Serial.print("SUM -> ");
  Serial.println(sum);
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
#endif
