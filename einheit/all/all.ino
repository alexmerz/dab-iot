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

// create sensor data struct instance
struct Sensordata sensordata;

// create sensor objects
FWAcc fwacc(500);
FWBaro fwbaro(1000);
FWHumi fwhumi(2000);
FWSound fwsound(400);
FWTouch fwtouch(500);
FWUv fwuv(500);
FWDust2 fwdust;
FWLight fwlight(1000);
FWGPS fwgps(1000);

// macros for hall sensor
#define HALL_PIN 4
#define TRACKING 1

// macros for gps module
#define GPS_VALID 1

// macros for sending data via mobile connection
#define SEND_INTERVAL 10000
unsigned long send_next = 0;

void onSensor(Framework &sensor);

void setup() {
  Serial.begin(9600);

  // initialize log file object
  LSD.begin();

  // initialize hall sensor
  hallInit(HALL_PIN);

  // get deviceId
  initDeviceId();
  String did = "\"";
  did += getDeviceId();
  did += "\"";
  sensordata.deviceid = did;

  // initialize tourId as empty
  sensordata.tourid = "";

  // initialize sensors
  fwacc.init();
  fwbaro.init();
  fwhumi.init();
  fwsound.init();
  fwtouch.init(7);
  fwdust.init();
  fwuv.init();
  fwgps.init();
  fwlight.init();

  // initialize callbacks for sensors
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

    // check sensors for data
    fwacc.check();
    fwbaro.check();
    fwhumi.check();
    fwsound.check();
    fwtouch.check();
    fwdust.check();
    fwuv.check();
    fwlight.check();
    fwgps.check();

    // get current timestamp
    unsigned long currentTime = millis();

    // wait for stable gps signal
    if (fwgps.isValid() != GPS_VALID) {
      return;
    }
    Serial.println("GPS found.");

    // wait for hall sensor response
//    if (hallCheck() != TRACKING) {
//      sensordata.tourid = "";
//      return;
//    }
//    Serial.println("Tracking active.");

    // create new tourId when previous tour is over
    if (sensordata.tourid == "") {
        sensordata.tourid = createTourId(fwgps);
        Serial.println("Created new tour id.");
        Serial.print("TourId: ");
        Serial.println(sensordata.tourid);
    }

    // wait for next sending time
    if (currentTime < send_next) {
      Serial.println("Next send not reached.");
      return;
    }

    // write data to log file when no data connection
    if (sendData(sensordata) != 0) {
       saveData(sensordata);
       Serial.println("No data connection. Writing to log file.");
       return;
    }
    Serial.println("Data sent.");

    // get new sending time
    send_next = currentTime + SEND_INTERVAL; 

    // write data to serial
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
  }
}

