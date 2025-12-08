/*****************************************
 * 
 * Serial Port Source File 
 * 
 * Contaisn Mutexes and Stuff
 *****************************************/

#include "serialUSLI.h"
#include "defines.h"
#include <arduino_freertos.h>

/******** STATIC VARS ********/
static SemaphoreHandle_t usbSerialMutex;

/******** EXTERNAL FUNCTIONS ********/
void initUSB()
{
    usbSerialMutex = xSemaphoreCreateMutex();
}

bool openUSB()
{
    return ((xSemaphoreTake(usbSerialMutex, portMAX_DELAY) == pdTRUE));
}

bool closeUSB()
{
    return ((xSemaphoreGive(usbSerialMutex) == pdTRUE));
}