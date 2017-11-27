#include <LWiFi.h>
#include <LWiFiClient.h>

#include "FWAcc.h" // I2C
#include "FWGPS.h" // Intern
#include "FWSound.h" // A0
#include "FWUv.h" // A1
#include "FWLight.h" // A2

#define AP "WlanSSID"
#define PW "Passwort"

LWiFiClient c;

FWAcc fwacc(200);
FWGPS fwgps(1000);
FWSound fwsound(1000);
FWUv fwuv(1000);
FWLight fwlight(1000);

struct Sensordata {
  String acc;
  String gps;
  String sound;
  String uv;
  String light;
} sensordata;

#define SEND_INTERVAL 3000L
unsigned long send_next = 30000;

void setup() {
  LWiFi.begin();

  fwacc.init();
  fwgps.init();
  fwsound.init();
  fwuv.init();
  fwlight.init();

  fwacc.setCallback(onSensor);
  fwgps.setCallback(onSensor);
  fwsound.setCallback(onSensor);
  fwuv.setCallback(onSensor);
  fwlight.setCallback(onSensor);

  LWiFi.connect(AP, LWiFiLoginInfo(LWIFI_WPA, PW));
//  LWiFi.connect("MeinTelefon");

}

void loop() {
  fwacc.check();
  fwgps.check();
  fwsound.check();
  fwuv.check();
  fwlight.check();

  if (millis() > send_next) {
    send_next = millis() + SEND_INTERVAL;
    sendData(sensordata);
  }
}

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
  }
  /*
    Serial.print("ZACC: ");
    Serial.println(sensordata.acc);
    Serial.print("UV: ");
    Serial.println(sensordata.uv);
    Serial.print("LIGHT: ");
    Serial.println(sensordata.light);
    Serial.print("SOUND: ");
    Serial.println(sensordata.sound);
    Serial.print("GPS: ");
    Serial.println(sensordata.gps);
  */
}

void sendData(struct Sensordata sensordata) {
  String request = "GET /am-dab/write.php?d={\"token\":\"11\",\"temp\":\"0\",\"humi\":\"0\",\"dust\":\"0.0\",\"press\":\"0\",";
  request += sensordata.acc;
  request += ",";
  request += sensordata.gps;
  request += ",";
  request += sensordata.light;
  request += ",";
  request += sensordata.uv;
  request += ",";
  request += sensordata.sound;
  request += "} HTTP/1.1";

  c.connect("p435939.webspaceconfig.de", 80);
  c.println(request);
  c.println("Host: p435939.webspaceconfig.de");
  c.println();
  c.stop();
  /*
    while (!c.available())
    {
      delay(10);
    }
    while (c)
    {
      int v = c.read();
      if (v != -1)
      {
        Serial.print((char)v);
      }
      else
      {
        Serial.println("no more content, disconnect");
        c.stop();
      }
    }
  */

}

