/**
 * Hauptprogramm
 */

// Inkludiere die Klasse für den Beschleunigungssensor
#include "FWAcc.h"
#include "FWSound.h"

// Erzeuge eine Instanz für den Beschleunigungssensor
// er soll alle 500 MS abgefragt werden
FWAcc fwacc(500);
FWSound fwsound(100);

void setup() {
  Serial.begin(9600);

  // Initialiserung des Sensors
  fwacc.init();
  fwsound.init();
  // die Funktion onSensor soll aufgerufen werden, 
  // wenn ein Messzeitpunkt eintritt
  fwacc.setCallback(onSensor);
}

void loop() {
  // Pruefe, ob ein Messzeitpunkt vorliegt
  fwacc.check();
  fwsound.check();
}

/**
 * Die Funktion wird aufgerufen, wenn ein Messzeitpunkt
 * erreicht wurde.
 * Der uebergebene Parameter sensor ist die Instanz der Sensorklasse,
 * welche den Messzeitpunkt ausgelöst hat
 */
void onSensor(Framework &sensor) 
{  
  // FWACCTYPE ist eine Konstante, welche im Header
  // der FWAcc-Klasse definiert wurde.
  if(FWACCTYPE == sensor.getType()) {
      Serial.print("Beschleunigung: ");
  }

  else if(sensor.getType() == FWSOUNDTYPE) {
    Serial.print("Lautstärke: ");
  }
  
  // Sensordaten auf dem seriellen Monitor ausgeben
  Serial.println(sensor.getData());
}
