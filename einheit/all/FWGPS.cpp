#ifndef FWGPS_cpp
#define FWGPS_cpp

#include "Arduino.h"
#include <LGPS.h>
#include "Framework.h"
#include "FWGPS.h"

FWGPS::FWGPS(unsigned long duration):
Framework(duration)
{
    _duration = duration;
}

void FWGPS::init()
{  
  LGPS.powerOn();  
  LGPS.setMode(GPS_FULL_START);
  delay(500);
}

void FWGPS::on() {
  LGPS.powerOn();
}

void FWGPS::off() {
  LGPS.powerOff();
}

int FWGPS::isValid() {
    return (valid == "A") ? 1 : 0;
}

String FWGPS::getData()
{
  time = "";
  date = "";
  latitude = "";
  longitude = "";
  altitude = "";
  valid = "A";
  gpsSentenceInfoStruct info;
  LGPS.getData(&info);  
  gprmc = (char *)info.GPRMC;
  gpgga = (char *)info.GPGGA;
  
  int from = 0;
  int index = gpgga.indexOf(',');
  from = index+1;
  index = gpgga.indexOf(',', from); // Uhrzeit steht zwischen 1. und 2. Komma
  time = gpgga.substring(from, index);
  
  from = index+1;
  index = gpgga.indexOf(',', from);
  latitude = gpgga.substring(from, index); // Lat zwischen 2. und 3. Komma

  from = index+1;
  index = gpgga.indexOf(',', from);
  latitude.concat(gpgga.substring(from, index)); // N oder S, zwischen 3. und 4. 

  from = index+1;
  index = gpgga.indexOf(',', from);
  longitude = gpgga.substring(from, index); // Long zwischen 4. und 5. Komma

  from = index+1;
  index = gpgga.indexOf(',', from);
  longitude.concat(gpgga.substring(from, index)); // W oder E, zwischen 5. und 6. 

  // drei Kommas überspringen
  for(int i = 0; i < 3; i++) {
    from = index+1;
    index = gpgga.indexOf(',', from);    
  }

  from = index+1;
  index = gpgga.indexOf(',', from);
  altitude = gpgga.substring(from, index); // Altitude zwischen 9. und 10. Komma

  // Alle weiteren Daten aus GPRMC
  from = 0;
  index = 0;  
  // zwei Kommas überspringen
  for(int i = 0; i < 2; i++) {
    from = index+1;
    index = gprmc.indexOf(',', from);    
  }

  from = index+1;
  index = gprmc.indexOf(',', from);
  valid = gprmc.substring(from, index); // Validity zwischen 2. und 3. Komma

  from = index+1;
  index = gprmc.indexOf(',', from);
  latitude = gprmc.substring(from, index); // Lat zwischen 3. und 4. Komma

  from = index+1;
  index = gprmc.indexOf(',', from);
  latitude.concat(gprmc.substring(from, index)); // N oder S, zwischen 4. und 5. 

  from = index+1;
  index = gprmc.indexOf(',', from);
  longitude = gprmc.substring(from, index); // Long zwischen 5. und 6. Komma

  from = index+1;
  index = gprmc.indexOf(',', from);
  longitude.concat(gprmc.substring(from, index)); // W oder E, zwischen 6. und 7. 

  // zwei Kommas überspringen
  for(int i = 0; i < 2; i++) {
    from = index+1;
    index = gprmc.indexOf(',', from);    
  }

  from = index+1;
  index = gprmc.indexOf(',', from);
  date = gprmc.substring(from, index); // Lat zwischen 9. und 10. Komma

  result = "\"location\": {\"type\": \"Point\", \"coordinates\": [";
  result += latitude;
  result += ",";
  result += longitude;
  result += "],";

  result += "\"altitude\":";
  result += altitude;
  result += "}";

  result += "\"dateTime\":";
  result += date;
  result += "T";
  result += time;
        
  return result;
}

void FWGPS::check() 
{
  // Wurde der nächste Messzeitpunkt bereits erreicht?
  if(millis() > _nextts)
  {
    // wenn ja, nächsten Messzeitpunkt errechnen
    _nextts = millis() + _duration;  
    // callback-Funktion aufrufen, wenn sie definiert wurde
    if(_callback) {
      // this verweist auf die Klassen-Instant selbst, mit dem Stern
      // übergeben wir einen Pointer auf diese Instanz statt dem Objekt
      // selbst
      _callback(*this);
    }
  }   
}

const char* FWGPS::getType()
{
  return FWGPSTYPE;
}
#endif
