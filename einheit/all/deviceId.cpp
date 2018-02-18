#include <vmsim.h>
#include <LTask.h>
#include <string>
#include <iostream>
#include <cstdlib>
#include "deviceId.h"
#include "sensordata.h"
#include "FWGPS.h"

#ifdef __cplusplus
extern "C" {
#endif

using namespace std;

#define DEVICE_ID_LEN (15 + 1)

static char deviceId[DEVICE_ID_LEN];

boolean handleDeviceId(void *deviceId) {
    char *imeiFromOs = NULL;
    while((imeiFromOs = vm_get_imei()) == NULL)
        ;
    strncpy((char *)deviceId, imeiFromOs, DEVICE_ID_LEN);
}

void initDeviceId(struct Sensordata sensordata) {
    LTask.remoteCall(&handleDeviceId, (void *)deviceId);
    String did = "\"";
    did = getDeviceId();
    did += "\"";
    sensordata.deviceid = did;
}

char *getDeviceId(void) {
    return deviceId;
}

String createTourId(FWGPS g) {
    String gpsResponse;
    gpsResponse = g.getData();
    if (!gpsResponse) {
        Serial.println("No gps response.");
        return "";
    }
    Serial.println(gpsResponse);
    int pos = 0;
    pos = gpsResponse.indexOf("\"dateTime\":");
    String dateTimePos = gpsResponse.substring(pos);

    String dateTime = dateTimePos.substring(dateTimePos.length() - 12, 12);
    
    String tourId = "";
    tourId += getDeviceId();
    tourId += "-";
    tourId += dateTime;

    return tourId;
}

#ifdef __cplusplus
}
#endif
