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

/************** EXTERNAL FUNCTIONS **************/
void loggerTask(void *pvParameters);
bool storeData();

#endif // LOGGER_H
