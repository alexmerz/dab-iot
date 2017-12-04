#include "FWDust2.h"

FWDust2 fwdust;

void setup() {
  Serial.begin(300);
  fwdust.init();
  fwdust.setCallback(onSensor);
}

void loop() {
  fwdust.check();
}

void onSensor(Framework &sensor) {
  if (FWDUST2TYPE == sensor.getType()) {
    Serial.println(sensor.getData());
  }
}

