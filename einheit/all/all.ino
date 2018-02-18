#include "Grove_LED_Bar.h"
#include <LSD.h>

#include "FWAcc.h"
#include "FWBaro.h"
#include "FWSound.h"
#include "FWTouch.h"
#include "FWDust2.h"
#include "FWHumi.h"
#include "FWUv.h"
#include "FWGPS.h"
#include "FWLight.h"
#include "hall.h"
#include "sendData.h"
#include "sensordata.h"
#include "deviceId.h"

struct Sensordata sensordata;

Grove_LED_Bar ledbar(6,5,0);

FWAcc fwacc(500);
FWBaro fwbaro(1000);
FWHumi fwhumi(2000);
FWSound fwsound(400);
FWTouch fwtouch(500);
FWUv fwuv(500);
FWDust2 fwdust;
FWLight fwlight(1000);
FWGPS fwgps(1000);

int ctReset = 0;
unsigned long touchResetDuration = 5000; // reset Touch counter after 5 seconds
unsigned long nextTouchReset = 0;

#define HALL_PIN 4
#define TRACKING 1

#define GPS_VALID 1

#define SEND_INTERVAL 10000
unsigned long send_next = 0;

void resetCapture() {
  ctReset = 0;
  ledbar.setLevel(10);
  ledbar.setBits(0);
}

void onSensor(Framework &sensor);

void setup() {
  Serial.begin(9600);

  LSD.begin();

  hallInit(HALL_PIN);

  initDeviceId(sensordata);
  Serial.print("Device ID: ");
  Serial.println(getDeviceId());

  ledbar.begin();
  ledbar.setLevel(10);

  // Sensor init
  fwacc.init();
  fwbaro.init();
  fwhumi.init();
  fwsound.init();
  fwtouch.init(7);
  fwdust.init();
  fwuv.init();
  fwgps.init();
  fwlight.init();

  fwacc.setCallback(onSensor);
  fwbaro.setCallback(onSensor);
  fwhumi.setCallback(onSensor);
  fwsound.setCallback(onSensor);
  fwtouch.setCallback(onSensor);
  fwdust.setCallback(onSensor);
  fwuv.setCallback(onSensor);
  fwlight.setCallback(onSensor);  
  fwgps.setCallback(onSensor);
}

void loop() {
    fwacc.check();
    fwbaro.check();
    fwhumi.check();
    fwsound.check();
    fwtouch.check();
    fwdust.check();
    fwuv.check();
    fwlight.check();
    fwgps.check();

    unsigned long currentTime = millis();

//    if (fwgps.isValid() != GPS_VALID) {
//      return;
//    }
//    Serial.println("GPS found.");
//
//    String tourId = createTourId(fwgps);
//    Serial.print("TourId: ");
//    Serial.println(tourId);
    
//    if (hallCheck() != TRACKING) {
//      return;
//    }
//    Serial.println("Tracking active.");
//    
//    if (currentTime < send_next) {
//      Serial.println("Next send not reached.");
//      return;
//    }
//    
//    if (sendData(sensordata) != 0) {
//       saveData(sensordata);
//       Serial.println("No data connection. Writing to log file.");
//       return;
//    }
//    Serial.println("Data sent.");
//    
//    send_next = currentTime + SEND_INTERVAL; 
    
    Serial.println(formatData(sensordata));
}

void onSensor(Framework &sensor) {
  if (FWACCTYPE == sensor.getType()) {
    sensordata.acc = sensor.getData();
  } else if (FWBAROTYPE == sensor.getType()) {    
    sensordata.baro = sensor.getData(); 
  } else if (FWHUMITYPE == sensor.getType()) {    
    sensordata.humi = sensor.getData(); 
  } else if (FWGPSTYPE == sensor.getType()) {
    sensordata.gps = sensor.getData();
  } else if (FWSOUNDTYPE == sensor.getType()) {
    sensordata.sound = sensor.getData();
  } else if (FWUVTYPE == sensor.getType()) {
    sensordata.uv = sensor.getData();
  } else if (FWLIGHTTYPE == sensor.getType()) {
    sensordata.light = sensor.getData();
  } else if (FWDUST2TYPE == sensor.getType()) {
    sensordata.dust = sensor.getData();
  } else if (FWTOUCHTYPE == sensor.getType()) {          
      if(sensor.getData().equals("1")) {
        ctReset++;
        ledbar.setLevel(ctReset);
        nextTouchReset = millis()+touchResetDuration;
      }
  }
}

