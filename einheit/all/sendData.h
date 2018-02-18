#pragma once

#ifdef __cplusplus
extern "C" {
#endif

String formatData(struct Sensordata sensordata);
void saveData(struct Sensordata sensordata);
int sendData(struct Sensordata sensordata);

#ifdef __cplusplus
}
#endif
