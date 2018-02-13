#pragma once

#ifdef __cplusplus
extern "C" {
#endif

String formatData(struct Sensordata sensordata);
void saveData(struct Sensordata sensordata);
void sendDataBT(struct Sensordata sensordata);
void sendData(struct Sensordata sensordata);

#ifdef __cplusplus
}
#endif
