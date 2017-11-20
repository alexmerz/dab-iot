/**
 * Hauptprogramm
 */

// Inkludiere die Klasse für den Feinstaubsensor
#include "FWDust.h"

// Erzeuge eine Instanz für den Feinstaubsensor
// er soll alle 30000 MS abgefragt werden
FWDust fwdust(30000);

void setup() {
  Serial.begin(9600);

  // Initialiserung des Sensors
  fwdust.init();
  // die Funktion onSensor soll aufgerufen werden,
  // wenn ein Messzeitpunkt eintritt
  fwdust.setCallback(onSensor);
}

void loop() {
  // Pruefe, ob ein Messzeitpunkt vorliegt
  fwdust.check();
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
  // der FWDust-Klasse definiert wurde.
  if(FWDUSTTYPE == sensor.getType()) {
      Serial.println("Feinstaubkonzentration:");
  }

  // Sensordaten auf dem seriellen Monitor ausgeben
  Serial.println(sensor.getData());
}
