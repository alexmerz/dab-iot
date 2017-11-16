// Includes
// Library
#include <LSD.h>
#include <Grove_LED_Bar.h>

// Header files
#include "FWAcc.h"
#include "FWTouch.h"

Grove_LED_Bar ledbar(6,5,0);

FWAcc fwacc(500);
FWTouch fwtouch(1000);

int ctReset = 0;
unsigned long touchResetDuration = 2000; // Touch-Counter nach dieser Zeit resetten
unsigned long nextTouchReset = 0;

char* logfile = "log.csv";
unsigned long saveDuration = 500; // Speichere Daten alle x Millisekunden
unsigned long nextSave = 0;

struct Sensordata {
  String touch;
  String acc;
} sensordata;

void setup() {
  ledbar.begin();

  ledbar.setLevel(10);
  LSD.begin();

  Serial.begin(9600);

  // init
  fwacc.init();
  fwtouch.init(7);

  // setCallback
  fwacc.setCallback(onSensor);
  fwtouch.setCallback(onSensor);

  // resetCapture
  resetCapture();
  ledbar.setBits(0);
}

void loop() {

  // check
  fwacc.check();
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

void onSensor(Framework &sensor)
{
  if(FWACCTYPE == sensor.getType()) {
    sensordata.acc = sensor.getData();
  } else if(FWTOUCHTYPE == sensor.getType()) {
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
  dataFile.println(data);
  dataFile.close();

}
