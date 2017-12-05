#include <LWiFi.h>
#include <LWiFiClient.h>
#include <LGPRS.h>    
#include <LGPRSClient.h>
#include <LSD.h>
#include "Grove_LED_Bar.h"
#include <vmsim.h>
#include <LTask.h>
#include <string.h>

#include "FWAcc.h"
#include "FWSound.h"
#include "FWTouch.h"
#include "FWDust2.h"
#include "FWUv.h"
#include "FWGPS.h"
#include "FWLight.h"

#define AP "dab-studenten-PLU8"
#define PW "DABHochschule"

LGPRSClient c;

Grove_LED_Bar ledbar(6,5,0);

FWAcc fwacc(500);
FWSound fwsound(100);
FWTouch fwtouch(1000);
FWUv fwuv(500);
FWDust2 fwdust;
FWLight fwlight(1000);
FWGPS fwgps(1000);

int ctReset = 0;
unsigned long touchResetDuration = 2000; // Touch-Counter nach dieser Zeit resetten
unsigned long nextTouchReset = 0;

#define SEND_INTERVAL 1
unsigned long send_next = 0;

struct Sensordata {
  String touch;
  String acc;
  String sound;
  String dust;
  String uv;
  String gps;
  String light;
  String deviceid;
} sensordata;

#define DEVICE_ID_LEN (15 + 1)

static char deviceId[DEVICE_ID_LEN];

boolean handleDeviceId(void *deviceId) {
    char *imeiFromOs = NULL;
    while((imeiFromOs = vm_get_imei()) == NULL)
        ;
    strncpy((char *)deviceId, imeiFromOs, DEVICE_ID_LEN);
}

void initDeviceId(void) {
    LTask.remoteCall(&handleDeviceId, (void *)deviceId);
}

char *getDeviceId(void) {
    return deviceId;
}

void resetCapture() {
  ctReset = 0;
  ledbar.setLevel(10);
  // TODO: Senden stoppen/starten
  ledbar.setBits(0);
}

void setup() {
  Serial.begin(9600);

  initDeviceId();
  Serial.print("Device ID: ");
  Serial.println(getDeviceId());

  String did = "\"token\":\"";
  did += getDeviceId();
  did += "\"";
  sensordata.deviceid = did;
  ledbar.begin();
  ledbar.setLevel(10);
  
  LWiFi.connect(AP, LWiFiLoginInfo(LWIFI_WPA, PW));
//  LSD.begin();

  // Initialiserung des Sensors
  fwacc.init();
  fwsound.init();
  fwtouch.init(7);
  fwdust.init();
  fwuv.init();
  fwgps.init();
  fwlight.init();

  fwacc.setCallback(onSensor);
  fwsound.setCallback(onSensor);
  fwtouch.setCallback(onSensor);
  fwdust.setCallback(onSensor);
  fwuv.setCallback(onSensor);
  fwlight.setCallback(onSensor);  
  fwgps.setCallback(onSensor);  

  resetCapture();
  ledbar.setBits(0);

}

void loop() {
  fwacc.check();
  fwsound.check();
  fwtouch.check();
  fwdust.check();
  fwuv.check();

  unsigned long currentTime = millis();

  if(nextTouchReset < currentTime) {
    ctReset = 0;
    nextTouchReset = millis()+touchResetDuration;
    ledbar.setLevel(0);
  }

  if (millis() > send_next) {
    send_next = millis() + SEND_INTERVAL;
    sendData(sensordata);
  }
  
}
/*
  if(nextSave < currentTime) {
    nextSave = currentTime + saveDuration;
    saveData(currentTime);
  }
}
*/
void onSensor(Framework &sensor) {
  if (FWACCTYPE == sensor.getType()) {
    sensordata.acc = sensor.getData();
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
  Serial.println(sensor.getData());  
}


 
void sendData(struct Sensordata sensordata) {
  String request = "GET /am-dab/write.php?d={\"temp\":\"0\",\"humi\":\"0\",\"press\":\"0\",";
  request += sensordata.acc;
  request += ",";
  request += sensordata.gps;
  request += ",";
  request += sensordata.light;
  request += ",";
  request += sensordata.uv;
  request += ",";
  request += sensordata.dust;
  request += ",";  
  request += sensordata.sound;
  request += ",";  
  request += sensordata.deviceid;  
  request += "} HTTP/1.1";
  Serial.println(request);
  Serial.println(c.connect("p435939.webspaceconfig.de", 80));
  c.println(request);
  c.println("Host: p435939.webspaceconfig.de");
  c.println();
  c.stop();
}


