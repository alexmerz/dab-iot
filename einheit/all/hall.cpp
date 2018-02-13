#include "Arduino.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "hall.h"

struct hallSensor {

  int pin;

  int track_sensors;

  long change_interval;
  long last_state_change;

  int current_state;
  int last_state;

  unsigned long current_time;
};

static struct hallSensor sensor;

void hallInit(const int pin) {
  sensor.pin = pin;
  pinMode(sensor.pin, INPUT);

  sensor.track_sensors = 0;

  sensor.change_interval = 300000;
  sensor.last_state_change = 0;

  sensor.current_state = digitalRead(sensor.pin);
  sensor.last_state = digitalRead(sensor.pin);
}

static void hallUpdate(const unsigned long current_time) {
  sensor.last_state = sensor.current_state;
  sensor.last_state_change = current_time;
}

int hallCheck() {
  unsigned long current_time = millis();
  sensor.current_state = digitalRead(sensor.pin);
  
  if (sensor.current_state != sensor.last_state) {
    hallUpdate(current_time);
    }

  if (sensor.last_state_change != 0 && ((sensor.last_state_change + sensor.change_interval) >= current_time)) {
    sensor.track_sensors = 1;
    return sensor.track_sensors;
    }
  else { // ((sensor.last_state_change + sensor.change_interval) < current_time) {
    sensor.track_sensors = 0;
    return sensor.track_sensors;
    }
}

#ifdef __cplusplus
}
#endif
