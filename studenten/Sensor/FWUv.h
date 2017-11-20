#ifndef FWUv_h
#define FWUv_h

#include <Wire.h>
#include "Arduino.h"

#include "Framework.h"


#define FWUVTYPE "UV"

class FWAcc:public Framework
{
  public:
    FWUv(unsigned long duration);
    String getData();
    const char* getType();
    void init();
  private: // nur innerhalb der Klasse sichtbar
    /**
     * Achtung: hier wird lediglich der Speicher für das Objekt reserviert,
     * der eigentliche Konstruktor-Aufruf muss in der Implementierung erfolgen.
     * Die Deklaration
     * ADXL345 adxl();
     * würde unter Umständen fehlschlagen, abhängig von der Konstruktor der jeweiligen
     * Sensorbibliothek)
     */

};

#endif
