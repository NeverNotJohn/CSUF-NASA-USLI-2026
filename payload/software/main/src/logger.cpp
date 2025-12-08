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
#include "defines.h"
#include "sdfs.h"
#include "timeUSLI.h"
#include <stdint.h>
#include "serialUSLI.h"

/************** GLOBAL VARS **************/
TaskHandle_t loggerHandle = NULL;
DMAMEM DataPacketLog dataLog;

/************** STATIC VARS **************/

/************** STATIC FUNCS **************/
void storeDataRam(DataPacket data)
{

}

void transmitPacket(void *pvParameters)
{
    DataPacket *dataPtr = (DataPacket *)pvParameters;

    char buffer[128];
    String stringBuffer;

    snprintf(buffer, sizeof(buffer),
        "%lu %d %d %d %.2f %.4f %.4f %.0f %.0f %.0f",
        dataPtr->n,               // uint32_t counter
        dataPtr->hour,            // int
        dataPtr->min,             // int
        dataPtr->sec,             // int
        dataPtr->altitude_ft,     // float
        dataPtr->lng,             // float (6 decimal places)
        dataPtr->lat,             // float (6 decimal places)
        dataPtr->roll_deg,        // float
        dataPtr->pitch_deg,       // float
        dataPtr->yaw_deg          // float
    );

    // Is this OK?
    stringBuffer = String(buffer);
    txRYLR896(stringBuffer);

    delete dataPtr;               // Very uncomfy
    vTaskDelete(NULL);
}
/************** EXTERN FUNCS **************/
// Stores data onto RAM 
void loggerTask(void *pvParameters)
{
    ///////////// Setup /////////////

    // Init RYLR896
    initRYLR896();

    // Init Vars
    DataPacket currentData;
    dataLog.size = 0;

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
        currentData.roll_deg = -1.0;                                // Insert MPU6050 Data
        currentData.pitch_deg = -1.0;
        currentData.yaw_deg = -1.0;
        
        if (openUSB())
        {
            Serial.printf( "%i:\tTime (%i:%i:%i)\t\tAlt (ft) %.2f\t\tLng %f\t\tLat %f\t\tRoll Pitch Yaw (deg) (%.2f,%.2f,%.2f) \n",
                currentData.n,
                currentData.hour,
                currentData.min,
                currentData.sec,
                currentData.altitude_ft,
                currentData.lng,
                currentData.lat,
                currentData.roll_deg,
                currentData.pitch_deg,
                currentData.yaw_deg
            );
            closeUSB();
        }

        // Transmit Data every 10 seconds
        if ( (currentData.n % 20) == 0)
        {
            DataPacket *packetCopy = new DataPacket(currentData);
            xTaskCreate(transmitPacket, "Transmition Task", 4096, packetCopy, 1, NULL);
        }
        
        // Delay
        vTaskDelay(pdMS_TO_TICKS(LOGGER_PERIOD_MS));
    }
};