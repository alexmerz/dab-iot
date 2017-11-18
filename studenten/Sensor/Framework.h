/**
 * Basisklasse für alle Sensoren
 * 
 * Die Headerdatei enthölt lediglich die Definitionen für die Klasse,
 * keine Implementierung
 */

// wenn diese Konstante nicht definiert wurde,
// kompiliere den folgenden Code
#ifndef Framework_h
// dieser Mechanismus verhindert, das mehrfache Includes der selben
// Datei zu einer Fehlermeldung führen

// definiere die Konstante
#define Framework_h

// CPP und H-Dateien unterliegen nicht der Arduino-Magie
// deshalb binden wir die Arduino.h von Hand ein
#include "Arduino.h"

/** 
 * Vorab-Definition der Klasse Framework
 * Diese benötigen wir für die nachfolgende Typendefinition.
 * Denn diese Typendefinition wird selbst in der eigentlichen 
 * Klassendefinitition verwendet. Die Vorab-Definition hilft uns 
 * eine Huhn-Ei-Situation aufzulösen.
 */
class Framework;

/**
 * Wir benötigen eine Typdefinition, um die Callback-Funktion in 
 * einer Variable der Klasse zu vermerken.
 * Die Typendefinition namens event_framework_t beschreibt einen 
 * Pointer auf eine Funktion ohne Rueckgabewert (void), und dem Parameter
 * Framework, der auf die aufrufende Sensorklasse verweist
 */

//       Rueckgabewert
//       |    Typ ist Pointer
//       |    |Typname  
//       |    ||                  Typ repräsentiert eine Funktion
//       |    ||                  |          Parameter ist ein Pointer
//       |    ||                  |          |
typedef void (*event_framework_t) (Framework &);

// Eigentliche Klassendefinition
class Framework
{
  public: // für alle sichtbaren Methoden und Variablen    
    Framework(unsigned long duration); // Konstruktor   
    virtual void init(); // init()-Funktion    
    virtual String getData(); // Sensor auslesen/Daten ermitteln    
    virtual const char* getType(); // Klassentyp zurueckgeben   
    virtual void check(); // Messintervall einhalten    
    virtual void setCallback(event_framework_t fw); // Callbackfunktion setzen
  protected: // nur innerhalb der Basisklasse und abgeleiteten Klassen sichtbar
    unsigned long _duration; // Dauer des Intervalls
    unsigned long _nextts; // Naechster Messzeitpunkt
    event_framework_t _callback; // Callbackfunktion        
};
#endif

