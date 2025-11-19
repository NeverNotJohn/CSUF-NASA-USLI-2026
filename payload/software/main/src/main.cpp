#include "arduino_freertos.h"
#include "defines.h"
#include "motor.h"
#include "mpu6050.h"
#include "bmp280.h"
#include <stdint.h>
#include <stdlib.h>

using namespace arduino;

/************** TASK HANDLES **************/
TaskHandle_t blinkyHandle = NULL;

/************** GLOBAL VARS **************/
uint32_t n = 0;
DMAMEM DataPacket dataBuffer[DATA_BUFFER_SIZE];

/************** MUTEXES **************/
// FIXME put in separate files
SemaphoreHandle_t bmp280Mutex;
SemaphoreHandle_t neo6mMutex;
SemaphoreHandle_t mpu6050Mutex;
SemaphoreHandle_t soilMutex;

/************** HELPER FUNCTIONS **************/
// Beeps
void beep(int numBeeps, double onInterval_ms, double offInterval_ms)
{

    if (!ANNOY_CYAN) return;

    const TickType_t offInterval = offInterval_ms / portTICK_PERIOD_MS; 
    const TickType_t onInterval = onInterval_ms / portTICK_PERIOD_MS; 

    for (int i = 0; i < numBeeps; i++)
    {
        digitalWrite(BUZZ_PIN, 1);
        vTaskDelay(onInterval);
        digitalWrite(BUZZ_PIN, 0);
        vTaskDelay(offInterval);
    }
}



/************** THREADS **************/
// Sanity Check
void blinkyTask(void *pvParameters)
{
    // Block for 1000 ms
    const TickType_t xDelay = 1000 / portTICK_PERIOD_MS; 
    bool ledStatus = 0;

    for (;;)
    {
        digitalWrite(LED_OUTPUT_PIN, ledStatus);
        beep(1, 100, 0);
        ledStatus = !ledStatus;
        vTaskDelay(xDelay);
    }
}

// FIXME Put in separate file?
// Reads data and yeets into memory
void sensorThread(void *pvParamters)
{
    DataPacket currentData;

    // Index and time
    currentData.index = n++;
    currentData.hour = 0;
    currentData.min = 0;
    currentData.sec = 0;

    // Get Altitude
    if (xSemaphoreTake(bmp280Mutex, portMAX_DELAY) == pdTRUE)
    {
        currentData.altitude = 0;
        xSemaphoreGive(bmp280Mutex);
    }

    // Get GPS Coords
    if (xSemaphoreTake(neo6mMutex, portMAX_DELAY) == pdTRUE)
    {
        currentData.latitude = 0;
        currentData.longitude = 0;
        xSemaphoreGive(neo6mMutex);
    }

    // Get Orientation
    if (xSemaphoreTake(mpu6050Mutex, portMAX_DELAY) == pdTRUE)
    {
        currentData.roll = 0;
        currentData.pitch = 0;
        currentData.yaw = 0;
        xSemaphoreGive(mpu6050Mutex);
    }

    // Get Soil Sensors
    if (xSemaphoreTake(soilMutex, portMAX_DELAY) == pdTRUE)
    {
        currentData.pH = 0;
        currentData.EC = 0;
        xSemaphoreGive(soilMutex);
    }

    // Log Data onto RAM
}



void setup()
{
    printf("Program Started!");

    // PIN ASSIGNMENT
    Serial.begin(USB_BAUD_RATE);
    pinMode(LED_OUTPUT_PIN, OUTPUT);
    pinMode(BUZZ_PIN, OUTPUT);

    // MUTEX INIT

    // DEVICE INIT
    // initMPU6050();

    // TASK CREATION
    //xTaskCreate(testBMP, "BMP280 Test Task", 4096, NULL, 1, &bmpTestHandle);
    xTaskCreate(blinkyTask, "Blinky Task", 4096, NULL, 1, &blinkyHandle);
}

void loop()
{
    vTaskStartScheduler();
    printf("Wtf just happened");                                    // Should never Happen
}

