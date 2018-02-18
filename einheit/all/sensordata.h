#pragma once

#ifdef __cplusplus
extern "C" {
#endif

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
  String tourid;
};

#ifdef __cplusplus
}
#endif
