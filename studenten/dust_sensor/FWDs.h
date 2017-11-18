/**
 * Header-Datei für die Sensorklasse des
 * Feinstaubsensor
 */

#ifndef FWDs_h
#define FWDs_h

// bereits Teil des Arduino-SDKs
#include <Wire.h>

// heruntergeladene Bibliothek
// installiert im Arduino-Verzeichnis unter libraries
// #include <filename.h>

#include "Arduino.h"

// Es reicht die Header-Datei der Basisklasse zu inkludieren
// die zugehörige CPP-Datei fügt der Prä-Compiler automatisch
// hinzu, wenn sie den gleichen Namen trägt
#include "Framework.h"

// Konstante, um die Klasse später einfach zu identifizieren
#define FWDSTYPE "DS"

// FWDs erbt die öffentlichen Methoden von Framework
class FWDs:public Framework
{
  public:
    FWDs(unsigned long duration);
    String getData();
    const char* getType();
    void init();
  private: // nur innerhalb der Klasse sichtbar
//    PPD42NS ppd; // Sensor-Objekt  unsigned long sample_duration;
  unsigned long sample_duration;
  unsigned long sampletime_ms;
  unsigned long lowpulseoccupancy;
  float ratio;
  float concentration;
  float sum;
  int i;
  float avg;

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
