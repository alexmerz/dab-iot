/**
 * Implementierung der Basisklasse
 */

#ifndef Framework_cpp
#define Framework_cpp

#include "Arduino.h"
#include "Framework.h"

/**
 * Konstruktor-Implementierung
 * Achtung: Im Konstruktor dürfen nur Variablenzuweisungen vorgenommen werden!
 * Der Sensor ist in diesem Stadium noch nicht ansprechbar
 */
Framework::Framework(unsigned long duration)  
{
  _duration = duration;
  _nextts = 0;
  _callback = NULL; 
}

/**
 * Hier findet die eigentliche Sensorinitialiserung statt
 */
void Framework::init() 
{
}

/**
 * Liefert den zuletzt ermittelnden Sensorwert
 */
String Framework::getData()
{
  return String("FW");
}

/**
 * Typ der Klasse als Zeichenkette in Form eines char-Array
 * Ein char[]-Array ist NICHT identisch mit einem Objekt der
 * Klasse String wie bei getData(). String wird durch Arduino.h
 * bereitgestellt, ein char-Array ist ein eingebauter C-Datentyp.
 */
const char* Framework::getType()
{
  return "FW";
}

/**
 * Prüft, ob ein Messzeitpunkt vorliegt
 */
void Framework::check() 
{
  // Wurde der nächste Messzeitpunkt bereits erreicht?
  if(millis() > _nextts)
  {
    // wenn ja, nächsten Messzeitpunkt errechnen
    _nextts = millis() + _duration;  

    // callback-Funktion aufrufen, wenn sie definiert wurde
    if(_callback) {
      // this verweist auf die Klassen-Instant selbst, mit dem Stern
      // übergeben wir einen Pointer auf diese Instanz statt dem Objekt
      // selbst
      _callback(*this);
    }
  }   
}

/** 
 * Setzt die Callback-Funktion
 */
void Framework::setCallback(event_framework_t fw) {
  _callback = fw;
}

#endif
