// #define CGPRS
#define CWIFI

#include <LBT.h>
#include <LBTServer.h>
#include "Grove_LED_Bar.h"
#include <vmsim.h>
#include <LTask.h>
#include <string.h>

#ifdef CWIFI
#include <LWiFi.h>
#include <LWiFiClient.h>
#endif

#ifdef CGPRS
#include <LGPRS.h>    
#include <LGPRSClient.h>
#endif


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

#ifdef CGPRS
#define VODAFONE "m2m.vodafone.de"
#define VODAFONE_USERNAME ""
#define VODAFONE_PASSWORD
#define TELEKOM "internet.telekom"
#define TELEKOM_USERNAME "t-mobile"
#define TELEKOM_PASSWORD "tm"
#define AP VODAFONE 
#define USERNAME VODAFONE_USERNAME
#define PW VODAFONE_PASSWORD

LGPRSClient client;
#endif

#ifdef CWIFI
#define AP "SSID"
#define PW "Password"

LWiFiClient client;
#endif


Grove_LED_Bar ledbar(6,5,0);

FWAcc fwacc(500);
FWBaro fwbaro(1000);
FWHumi fwhumi(2000);
FWSound fwsound(100);
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

#define SEND_INTERVAL 1000L
unsigned long send_next = 0;

boolean radioState = 0;
boolean btState = 0;

struct Sensordata {
  String touch;
  String acc;
  String baro;  
  String humi;
  String sound;
  String dust;
  String uv;
  String gps;
  String light;
  String deviceid;
} sensordata;

void sendData(struct Sensordata sensordata);
void sendDataBT(struct Sensordata sensordata);

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

  hallInit(HALL_PIN);

  initDeviceId();
  Serial.print("Device ID: ");
  Serial.println(getDeviceId());

  ledbar.begin();
  ledbar.setLevel(10);

  String did = "\"";
  did = getDeviceId();
  did += "\"";
  sensordata.deviceid = did;

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

  resetCapture();
  ledbar.setBits(0);

  sensordata.dust = "\"dust\":\"0\"";
  sensordata.humi = "\"humidity\":\"0\"";

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
        #ifdef CWIFI
        LWiFi.connect(AP, LWiFiLoginInfo(LWIFI_WPA, PW));
        #endif

        #ifdef CGPRS
        LGPRS.attachGPRS(AP, USERNAME, PW);
        #endif        
        
      } else {
        #ifdef CWIFI
        if(LWIFI_STATUS_CONNECTED == LWiFi.status()) {
          LWiFi.disconnect();
        }
        #endif
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

    if (fwgps.isValid() == GPS_VALID) {
        if (hallCheck() == TRACKING) {
          if (millis() > send_next) {
            #ifdef CWIFI
            if (LWIFI_STATUS_CONNECTED == LWiFi.status()) {      
            #endif
                #ifdef CGPRS
                if (radioState) {
                #endif
                    sendData(sensordata);      
                    send_next = millis() + SEND_INTERVAL;
                }    
            }
        
          if (!radioState && !btState) { // print data to serial, when no other option is available
            Serial.println(formatData(sensordata));
          }
          
          }
        }
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

  sendDataBT(sensordata);
  
}


String formatData(struct Sensordata sensordata) {
  String request = "{\"deviceData\": {";
  request += sensordata.humi;
  request += ",";  
  request += sensordata.acc;
  request += ",";
  request += sensordata.baro;
  request += ",";
  request += sensordata.light;
  request += ",";
  request += sensordata.uv;
  request += ",";
  request += sensordata.dust;
  request += ",";  
  request += sensordata.sound;
  request += "},";
  request += "\"deviceId\":\"";
  request += sensordata.deviceid;
  request += ",";
//  request += "\"tourId\":\"";
//  request += sensordata.deviceid;
//  request += "-";
//  request += fwgps.getTime();
//  request += ",";
  request += sensordata.gps;
  request += "}";

  return request;
}

void sendDataBT(struct Sensordata sensordata) {
  if(LBTServer.connected()) {
    String str = formatData(sensordata);
    str += "\n";
    LBTServer.write((uint8_t*)str.c_str(), str.length());                  
  }
}
 
void sendData(struct Sensordata sensordata) {
  String request = "GET /am-dab/write.php?d=";
  request += formatData(sensordata);
  request += " HTTP/1.1";
  Serial.println(request);

  Serial.println(client.connect("p435939.webspaceconfig.de", 80));
  client.println(request);
  client.println("Host: p435939.webspaceconfig.de");
  client.println();
  client.stop();

}


