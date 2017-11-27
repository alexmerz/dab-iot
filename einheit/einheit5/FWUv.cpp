/**
 * Implementierung der Sensorklasse für den
 * Beschleunigungsmesser
 */

#ifndef FWUv_cpp
#define FWUv_cpp

#include "Arduino.h"
#include "FWUv.h"

#define UV_PIN A1

// Unser Konstruktor ruft den Konstruktor
// der Basisklasse auf und den Konstruktor des adxl-Instanz unseres Objekts
FWUv::FWUv(unsigned long duration):
Framework(duration)
{
}

void FWUv::init()
{
  int sensorValue = 0;
  long uvIndex = 0;
}

String FWUv::getData()
{
  sensorValue = analogRead(UV_PIN);
  // Formel geaendert
  float uvIndex = (sensorValue*(5.0/1024.0)*307.0)/200.0;
  String s("\"uv\":\"");
  s+= uvIndex;
  s+= "\"";
  return s;
}

const char* FWUv::getType()
{
  return FWUVTYPE;
}
#endif
