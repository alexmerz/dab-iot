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
  for(int i=0;i<1024;i++)// accumulate readings for 1024 times
  {
      sensorValue=analogRead(DUST_PIN);
      sum=sensorValue+sum;
      delay(2);
  }
  long meanVal = sum/1024;  // get mean value
  long uvIndex = (meanVal*1000/4.3-83)/21;
  return meanVal;
}

const char* FWUv::getType()
{
  return FWUVTYPE;
}
#endif
