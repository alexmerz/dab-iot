#include "Arduino.h"
#include <LSD.h>

#ifdef __cplusplus
extern "C" {
#endif

//#define CGPRS
#define CWIFI

#include "sendData.h"
#include "sensordata.h"

#define LOGFILE "log.csv"

#include <LBT.h>
#include <LBTServer.h>

#ifdef CWIFI
#include <LWiFi.h>
#include <LWiFiClient.h>
#endif

#ifdef CGPRS
#include <LGPRS.h>    
#include <LGPRSClient.h>
#endif

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

void saveData(struct Sensordata sensordata) {
  LFile dataFile = LSD.open(LOGFILE, FILE_WRITE);

  String data = formatData(sensordata);
  
  dataFile.println(data);
  dataFile.flush();
  dataFile.close();
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

#ifdef __cplusplus
}
#endif
