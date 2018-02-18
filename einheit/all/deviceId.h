#pragma once

#include "FWGPS.h"

#ifdef __cplusplus
extern "C" {
#endif

boolean handleDeviceId(void *deviceId);
void initDeviceId(struct Sensordata sensordata);
char *getDeviceId(void);
String createTourId(FWGPS g);

#ifdef __cplusplus
}
#endif
