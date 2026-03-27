/*****************************************
 * 
 * Logger Header File
 * 
 *****************************************/

#ifndef LOGGER_H
#define LOGGER_H

#include "arduino_freertos.h"
#include "defines.h"

/************** EXTERN VARS **************/
extern TaskHandle_t loggerHandle;
extern DMAMEM DataPacketLog dataLog;
extern DMAMEM SoilPacketLog soilLog;

/************** EXTERNAL FUNCTIONS **************/
void loggerTask(void *pvParameters);
bool storeData(DataPacket data);
void storeSoilDataRam(SoilPacket data);

void initLogger();
MissionState getMissionState();
void setMissionState(MissionState state);

#endif // LOGGER_H
