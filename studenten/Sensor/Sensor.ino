/**
 * Hauptprogramm
 */
// Libraries
#include <LSD.h>
#include "Grove_LED_Bar.h"

// Sensor header files
#include "FWAcc.h"
#include "FWSound.h"
#include "FWTouch.h"

Grove_LED_Bar ledbar(6,5,0);

// Initialising sensor objects
FWAcc fwacc(500);
FWSound fwsound(100);
FWTouch fwtouch(1000);

// Variables for touch sensor reset
int ctReset = 0;
unsigned long touchResetDuration = 2000; // Touch-Counter nach dieser Zeit resetten
unsigned long nextTouchReset = 0;

// Variables for data logging
char* logfile = "log.csv";
unsigned long saveDuration = 500; // Speichere Daten alle x Millisekunden
unsigned long nextSave = 0;

struct Sensordata {
  String touch;
  String acc;
  String sound;
} sensordata;

void setup() {
  Serial.begin(9600);

  ledbar.begin();
  ledbar.setLevel(10);

  LSD.begin();

  // Initialiserung des Sensors
  fwacc.init();
  fwsound.init();
  fwtouch.init(7);
  
  // die Funktion onSensor soll aufgerufen werden, 
  // wenn ein Messzeitpunkt eintritt
  fwacc.setCallback(onSensor);
  fwsound.setCallback(onSensor);
  fwtouch.setCallback(onSensor);

  resetCapture();
  ledbar.setBits(0);
}

void loop() {
  // Pruefe, ob ein Messzeitpunkt vorliegt
  fwacc.check();
  fwsound.check();
  fwtouch.check();

  unsigned long currentTime = millis();

  if(nextTouchReset < currentTime) {
    ctReset = 0;
    nextTouchReset = millis()+touchResetDuration;
    ledbar.setLevel(0);
  }

  if(nextSave < currentTime) {
    nextSave = currentTime + saveDuration;
    saveData(currentTime);
  }
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
  if(sensor.getType() == FWACCTYPE) {
      Serial.print("Beschleunigung: ");
  }

  else if(sensor.getType() == FWSOUNDTYPE) {
    Serial.print("Lautstärke: ");
  }

  else if(FWTOUCHTYPE == sensor.getType()) {
    sensordata.touch = sensor.getData();
    if(sensordata.touch.equals("1")) {
      nextTouchReset = millis()+touchResetDuration;
      ctReset++;
      if(ctReset>10) {
        resetCapture();
      }
      ledbar.setLevel(ctReset);
    }
  }
  
  // For debugging only: Sensordaten auf dem seriellen Monitor ausgeben
  Serial.println(sensor.getData());
}

void resetCapture() {
  ctReset = 0;
  ledbar.setLevel(10);
  if(LSD.exists(logfile)) {
      LSD.remove(logfile);
      delay(1000);
  }
  ledbar.setBits(0);
}

void saveData(unsigned long currenttime) {
  LFile dataFile = LSD.open(logfile, FILE_WRITE);
  String data = "";
  data += String(currenttime);
  data += ",";
  data += sensordata.acc;
  data += ",";
  data += sensordata.touch;
  data += ",";
  data += sensordata.sound;
  dataFile.println(data);
  dataFile.close();
}
