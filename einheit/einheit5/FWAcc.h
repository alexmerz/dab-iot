/**
 * Header-Datei für die Sensorklasse des
 * Beschleunigungssensor
 */

#ifndef FWAcc_h
#define FWAcc_h

// bereits Teil des Arduino-SDKs
#include <Wire.h>

// heruntergeladene Bibliothek
// installiert im Arduino-Verzeichnis unter libraries
#include "ADXL345.h"

#include "Arduino.h"

// Es reicht die Header-Datei der Basisklasse zu inkludieren
// die zugehörige CPP-Datei fügt der Prä-Compiler automatisch 
// hinzu, wenn sie den gleichen Namen trägt
#include "Framework.h"

// Konstante, um die Klasse später einfach zu identifizieren
#define FWACCTYPE "ACC"
#define FWACC_Z 1

// FWAcc erbt die öffentlichen Methoden von Framework
class FWAcc:public Framework
{
  public:
    FWAcc(unsigned long duration);
    String getData();
    const char* getType();
    void init();
  private: // nur innerhalb der Klasse sichtbar
    ADXL345 adxl; // Sensor-Objekt
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
