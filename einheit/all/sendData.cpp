#include "Arduino.h"
#include <LSD.h>
#include <LGPRS.h>    
#include <LGPRSClient.h>

#ifdef __cplusplus
extern "C" {
#endif

#include "sendData.h"
#include "sensordata.h"
#include "deviceId.h"

#define LOGFILE "log.csv"

#define VODAFONE "m2m.vodafone.de"
#define VODAFONE_USERNAME NULL
#define VODAFONE_PASSWORD NULL
#define TELEKOM "internet.telekom"
#define TELEKOM_USERNAME "t-mobile"
#define TELEKOM_PASSWORD "tm"
#define AP VODAFONE 
#define USERNAME VODAFONE_USERNAME
#define PW VODAFONE_PASSWORD

#define SERVER_NAME "stage-geobikeserver.herokuapp.com"
#define POST_URL "/test-sensordata"
#define POST_PORT 80


String formatData(struct Sensordata sensordata) {
  String request = "{\"deviceData\": {";
  request += sensordata.humi;
  request += ", ";  
  request += sensordata.acc;
  request += ", ";
  request += sensordata.baro;
  request += ", ";
  request += sensordata.light;
  request += ", ";
  request += sensordata.uv;
  request += ", ";
  request += sensordata.dust;
  request += ", ";  
  request += sensordata.sound;
  request += "}, ";
  request += "\"deviceId\":";
  request += sensordata.deviceid;
  request += ", ";
  request += "\"tourId\":\"";
  request += sensordata.tourid;
  request += "\", ";
  request += sensordata.gps;
  request += "}";

  return request;
}

void saveData(struct Sensordata sensordata) {
  LFile dataFile = LSD.open(LOGFILE, FILE_WRITE);
  String data = formatData(sensordata);

  if (dataFile) {
    dataFile.println(data);
    dataFile.close();
  }
}

 
int sendData(struct Sensordata sensordata) {
  LGPRSClient client;
  int res = LGPRS.attachGPRS(AP, USERNAME, PW);
  if (!res) {
    return 1;
  }
  if (!client.connect(SERVER_NAME, POST_PORT)) {
    return 2;
  }

  client.println("POST " POST_URL " HTTP/1.1");
  client.println("Host: " SERVER_NAME);
  client.println("Connection: close");
  client.println("Content-Type: application/json");
  client.print("Content-Length: ");
  client.println(formatData(sensordata).length());
  client.println();
  client.println(formatData(sensordata));
  client.stop();
  
  return 0;
}

#ifdef __cplusplus
}
#endif
