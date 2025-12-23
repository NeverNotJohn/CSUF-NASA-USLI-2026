/*****************************************
 * 
 * I2C Source File
 * 
 *****************************************/

#include "i2c.h"
#include <arduino_freertos.h>
#include <semphr.h>

/************** STATIC VARS **************/
static SemaphoreHandle_t i2cMutex;

/************** FUNCTIONS **************/

// Initializes the i2cMutex
void initI2C()
{
    i2cMutex = xSemaphoreCreateMutex();
}

// Take the Semaphore Mutex
bool openI2C()
{
    return ((xSemaphoreTake(i2cMutex, portMAX_DELAY) == pdTRUE));
}

// Give Sempahore Mutex
bool closeI2C()
{
    return ((xSemaphoreGive(i2cMutex) == pdTRUE));
}


