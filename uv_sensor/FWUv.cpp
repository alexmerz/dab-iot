/**
 * Implementierung der Sensorklasse für den
 * Beschleunigungsmesser
 */

#ifndef FWUv_cpp
#define FWUv_cpp

#include "Arduino.h"
#include "FWAcc.h"

#define DUST_PIN A0

// Unser Konstruktor ruft den Konstruktor
// der Basisklasse auf und den Konstruktor des adxl-Instanz unseres Objekts
FWUv::FWUv(unsigned long duration):
Framework(duration)
{
}

void FWAcc::init()
{
  int value;
  long sum=0;
  long index=0;
}

String FWUv::getData()
{
  sensorValue=analogRead(DUST_PIN);

  long uvIndex = (sensorValue*1000/4.3-83)/21;
  return uvIndex;
}

const char* FWUv::getType()
{
  return FWUVTYPE;
}
#endif
