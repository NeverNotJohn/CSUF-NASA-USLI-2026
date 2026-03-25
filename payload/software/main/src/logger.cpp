/*****************************************
 * 
 * Logger Source File
 * 
 *****************************************/

#include "logger.h"
#include "arduino_freertos.h"
#include "RYLR896.h"
#include "bmp280.h"
#include "neo6m.h"
#include "mpu6050.h"
#include "defines.h"
#include "sdfs.h"
#include "timeUSLI.h"
#include <stdint.h>
#include "serialUSLI.h"

/************** GLOBAL VARS **************/
TaskHandle_t loggerHandle = NULL;
DMAMEM DataPacketLog dataLog;


/************** STATIC VARS **************/
static MissionState missionState = PRE_FLIGHT;
static SemaphoreHandle_t missionStateMutex; 

/************** STATIC FUNCS **************/
void storeDataRam(DataPacket data)
{
    if (dataLog.size >= DATA_LOG_SIZE) return;

    int i = dataLog.size;
    dataLog.data[i] = data;
    dataLog.size = dataLog.size + 1;
}

void transmitPacket(const DataPacket& data)
{
    char buffer[128];

    snprintf(buffer, sizeof(buffer),
        "%lu %d %d %d %.2f %.6f %.6f %.0f %.0f %.0f %d",
        data.n,
        data.hour,
        data.min,
        data.sec,
        data.altitude_ft,
        data.lng,
        data.lat,
        data.roll_deg,
        data.pitch_deg,
        data.yaw_deg,
        data.missionState
    );

    txRYLR896(buffer);  // Send directly
}

/************** EXTERN FUNCS **************/
// Stores data onto RAM 
void loggerTask(void *pvParameters)
{
    // Init RYLR896
    initRYLR896();

    // Init Vars
    DataPacket currentData;
    dataLog.size = 0;
    TickType_t xLastWakeTime = xTaskGetTickCount();

    // Sensor Data
    uint32_t counter = 0;
    float longitude;
    float latitude;

    for (;;)
    {
        // Retrieve Data from Sensors

        if (encodeGPS())
        {
            longitude =  getLongitude();
            latitude = getLatitude();
        }

        currentData.n = counter++;
        currentData.hour = hour();
        currentData.min = minute();
        currentData.sec = second();
        currentData.altitude_ft = getAltitude_ft();
        currentData.lng = longitude;
        currentData.lat = latitude;
       

        currentData.roll_deg =  getRoll();                                // Insert MPU6050 Data
        currentData.pitch_deg = getPitch();
        currentData.yaw_deg = getYaw();

        currentData.missionState = (uint8_t)getMissionState();
        
        if (openUSB())
        {
            Serial.printf( "%i:\tTime (%i:%i:%i)\t\tAlt (ft) %.2f\t\tLng %f\t\tLat %f\t\tRoll Pitch Yaw (deg) (%.2f,%.2f,%.2f)\t\t Mission State %i \n",
                currentData.n,
                currentData.hour,
                currentData.min,
                currentData.sec,
                currentData.altitude_ft,
                currentData.lng,
                currentData.lat,
                currentData.roll_deg,
                currentData.pitch_deg,
                currentData.yaw_deg,
                currentData.missionState
            );
            closeUSB();
        }

        // Transmit Data every 1 seconds
        if ( (currentData.n % 2) == 0)
        {
            transmitPacket(currentData);
        }

        // Store Data onto Ram
        MissionState temp = getMissionState();
        if (temp == IN_FLIGHT || temp == POST_FLIGHT)
        {
            storeDataRam(currentData);
        }

        // Delay
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(LOGGER_PERIOD_MS));
    }
};

void initLogger()
{
    missionStateMutex = xSemaphoreCreateMutex();
}

MissionState getMissionState()
{
    MissionState result = PRE_FLIGHT;
    if (xSemaphoreTake(missionStateMutex, portMAX_DELAY) == pdTRUE)
    {
        result = missionState;
        xSemaphoreGive(missionStateMutex);
    }
    return result;
}

void setMissionState(MissionState state)
{
    if (xSemaphoreTake(missionStateMutex, portMAX_DELAY) == pdTRUE)
    {
        missionState = state;
        xSemaphoreGive(missionStateMutex);
    }
}