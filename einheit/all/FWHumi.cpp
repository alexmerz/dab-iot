#ifndef FWHumi_cpp
#define FWHumi_cpp

#include "Arduino.h"
#include "FWHumi.h"
#include "DHT.h"

DHT dht(HUMIPIN, DHT22);

FWHumi::FWHumi(unsigned long duration):
    Framework(duration) {
}

void FWHumi::init() {
   dht.begin();
}

String FWHumi::getData() {
  // Es gibt zwei verschiedene DHT-Bibliotheken

  // Variante 1:
  //humidity = dht.readHumidity(); 

  // Variante 2:
  dht.readHT(&temperature, &humidity);

  String s("\"humidity\":\"");
   s.concat(humidity);
   s.concat("\",\"temperature:\":");
   s.concat(temperature);
   s.concat("\"");
  return s;
}

const char* FWHumi::getType() {
    return FWHUMITYPE;
}

#endif
