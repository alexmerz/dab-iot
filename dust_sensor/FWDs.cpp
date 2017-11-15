/**
 * Implementierung der Sensorklasse für den
 * Feinstaubsensor
 */

#ifndef FWDs_cpp
#define FWDs_cpp

#include "Arduino.h"
#include "FWDs.h"

#define DUST_PIN 8

// Unser Konstruktor ruft den Konstruktor
// der Basisklasse auf und den Konstruktor des ppd-Instanz unseres Objekts
FWDs::FWDs(unsigned long duration):
Framework(duration),
ppd()
{
}

void FWDs::init()
{
  unsigned long duration;
  unsigned long sampletime_ms = 30000;
  unsigned long lowpulseoccupancy = 0;
  float ratio = 0;
  float concentration = 0;
  float sum = 0;
  int i = 1;
  float avg = 0;
}

String FWDs::getData()
{
  // Die Werte auslesen
  duration = pulseIn(DUST_PIN, LOW);
  lowpulseoccupancy = lowpulseoccupancy+duration;
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

const char* FWDs::getType()
{
  return FWACCTYPE;
}
#endif
