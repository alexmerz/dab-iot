#include "FWDust2.h"

FWDust2 fwdust;

unsigned long testtime = 0;
bool state = 0;

void setup() {
  Serial.begin(300);
  fwdust.init();
  fwdust.setCallback(onSensor);
  pinMode(13, OUTPUT);
}

void loop() {
  fwdust.check();
  if(testtime < millis()) {
    digitalWrite(13, state);
    testtime = millis()+500;
    state = !state;
  }
}

void onSensor(Framework &sensor) {
  if (FWDUST2TYPE == sensor.getType()) {
    Serial.println(sensor.getData());
  }
}

