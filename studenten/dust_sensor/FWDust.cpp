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

  sampletime_ms = 30000;
  lowpulseoccupancy = 0;
  ratio = 0;
  concentration = 0;
  sum = 0;
  i = 1;
  avg = 0;
}

String FWDust::getData()
{
  // Die Werte auslesen
  sample_duration = pulseIn(DUST_PIN, LOW);
  lowpulseoccupancy = lowpulseoccupancy+sample_duration;
  ratio = lowpulseoccupancy/(sampletime_ms*10.0);  // Integer percentage 0=&gt;100
  concentration = 1.1*pow(ratio,3)-3.8*pow(ratio,2)+520*ratio+0.62; // using spec sheet curve

  // Summe der Konzentrationen
  sum += concentration;
  // Durchschnittswert ermitteln
  avg = sum / i;
  // Reset lowpulseoccupancy
  lowpulseoccupancy = 0;
  // i++
  i += 1;

  String s(avg);
  return s;
}

const char* FWDust::getType()
{
  return FWDUSTTYPE;
}
#endif
