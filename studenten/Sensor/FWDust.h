#ifndef FWDust_h
#define FWDust_h

#include "Arduino.h"
#include "Framework.h"

#define FWDUSTTYPE "DUST"

// FWDust erbt die öffentlichen Methoden von Framework
class FWDust:public Framework {
  
  public:
    FWDust(unsigned long duration);
    String getData();
    const char* getType();
    void init();
    void check();
  private: // nur innerhalb der Klasse sichtbar
//    PPD42NS ppd; // Sensor-Objekt  unsigned long sample_duration;
  unsigned long sample_duration;
  unsigned long sampletime_ms;
  unsigned long lowpulseoccupancy;
  float ratio;
  float concentration;
  int samples;
  uint32_t sum;

    /**
     * Achtung: hier wird lediglich der Speicher für das Objekt reserviert,
     * der eigentliche Konstruktor-Aufruf muss in der Implementierung erfolgen.
     * Die Deklaration
     * PPD42NS ppd();
     * würde unter Umständen fehlschlagen, abhängig von der Konstruktor der jeweiligen
     * Sensorbibliothek)
     */

};

#endif
