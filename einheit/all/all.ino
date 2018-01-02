#include <LBT.h>
#include <LBTServer.h>
#include <LWiFi.h>
#include <LWiFiClient.h>
#include <LGPRS.h>    
#include <LGPRSClient.h>
#include "Grove_LED_Bar.h"
#include <vmsim.h>
#include <LTask.h>
#include <string.h>

#include "FWAcc.h"
#include "FWBaro.h"
#include "FWSound.h"
#include "FWTouch.h"
#include "FWDust2.h"
#include "FWUv.h"
#include "FWGPS.h"
#include "FWLight.h"

#define AP "SSID"
#define PW "Passwort"

LGPRSClient c;

Grove_LED_Bar ledbar(6,5,0);

FWAcc fwacc(500);
FWBaro fwbaro(1000);
FWSound fwsound(100);
FWTouch fwtouch(500);
FWUv fwuv(500);
FWDust2 fwdust;
FWLight fwlight(1000);
FWGPS fwgps(1000);

int ctReset = 0;
unsigned long touchResetDuration = 5000; // Touch-Counter nach dieser Zeit resetten
unsigned long nextTouchReset = 0;

#define SEND_INTERVAL 1000L
unsigned long send_next = 0;

boolean radioState = 0;
boolean btState = 0;

struct Sensordata {
  String touch;
  String acc;
  String baro;  
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
  ledbar.setBits(0);
}

void setup() {
  Serial.begin(9600);

  initDeviceId();
  Serial.print("Device ID: ");
  Serial.println(getDeviceId());

  ledbar.begin();
  ledbar.setLevel(10);

  String did = "\"token\":\"";
  did += getDeviceId();
  did += "\"";
  sensordata.deviceid = did;

  // Initialiserung des Sensors
  fwacc.init();
  fwbaro.init();
  fwsound.init();
  fwtouch.init(7);
  fwdust.init();
  fwuv.init();
  fwgps.init();
  fwlight.init();

  fwacc.setCallback(onSensor);
  fwbaro.setCallback(onSensor);
  fwsound.setCallback(onSensor);
  fwtouch.setCallback(onSensor);
  fwdust.setCallback(onSensor);
  fwuv.setCallback(onSensor);
  fwlight.setCallback(onSensor);  
  fwgps.setCallback(onSensor);  

  resetCapture();
  ledbar.setBits(0);

  sensordata.dust = "\"dust\":\"0\"";

}

void loop() {
  
  fwacc.check();
  fwbaro.check();
  fwsound.check();
  fwtouch.check();
  fwdust.check();
  fwuv.check();
  fwlight.check();
  fwgps.check();

  unsigned long currentTime = millis();

  if(btState && !LBTServer.connected()) {
    LBTServer.accept(2);    
  }
  
  if(nextTouchReset < currentTime) {
    if(3 < ctReset && 6 >= ctReset) { // handle WIFI
      ledbar.setLevel(0);
      delay(100);
      ledbar.setLevel(ctReset);
      
      radioState = !radioState;
      if(radioState) {
        Serial.println(LWiFi.connect(AP, LWiFiLoginInfo(LWIFI_WPA, PW)));
      } else {
        if(LWIFI_STATUS_CONNECTED == LWiFi.status()) {
          LWiFi.disconnect();
        }
      }    
    }
    if(6 < ctReset) { // handle bluetooth      
      if(!btState) {
        Serial.println("BT warten");
        ledbar.setLevel(0);
        delay(100);
        ledbar.setLevel(ctReset);

        LBTServer.begin((uint8_t*)deviceId, (uint8_t*)"9876");
        btState = 1;
        LBTDeviceInfo info;
        if (LBTServer.getHostDeviceInfo(&info))
        {
          Serial.printf("LBTServer.getHostDeviceInfo [%02x:%02x:%02x:%02x:%02x:%02x]", 
            info.address.nap[1], info.address.nap[0], info.address.uap, info.address.lap[2], info.address.lap[1], info.address.lap[0]);
        }        
      } else {
        LBTServer.end();
        btState = 0;
        Serial.println("BT beendet");
      }
    }
    
    ctReset = 0;
    nextTouchReset = millis()+touchResetDuration;
    ledbar.setLevel(0);
  } else {
    if(btState) {
      ledbar.setLed(2,1);
    }
    if(radioState) {
      ledbar.setLed(1,1);
    }    
  }

  if (millis() > send_next) {
    if(LWIFI_STATUS_CONNECTED == LWiFi.status()) {      
      sendData(sensordata);      
      send_next = millis() + SEND_INTERVAL;
    }    
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
  } else if (FWBAROTYPE == sensor.getType()) {    
    sensordata.baro = sensor.getData(); 
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

  sendDataBT(sensordata);
  
}


String formatData(struct Sensordata sensordata) {
  String request = "{\"humi\":\"0\",";
  request += sensordata.acc;
  request += ",";
  request += sensordata.baro;
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
  request += "}";

  return request;
}

void sendDataBT(struct Sensordata sensordata) {
  if(LBTServer.connected()) {
    String str = formatData(sensordata);
    str += "\n";
    LBTServer.write((uint8_t*)str.c_str(), str.length());                  
    Serial.println(str);
  }
}
 
void sendData(struct Sensordata sensordata) {
  String request = "GET /am-dab/write.php?d=";
  request += formatData(sensordata);
  request += " HTTP/1.1";
  Serial.println(request);

  Serial.println(c.connect("p435939.webspaceconfig.de", 80));
  c.println(request);
  c.println("Host: p435939.webspaceconfig.de");
  c.println();
  c.stop();

}


