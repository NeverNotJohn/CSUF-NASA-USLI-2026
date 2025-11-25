#include "arduino_freertos.h"
#include "defines.h"
#include "motor.h"
#include "mpu6050.h"
#include "bmp280.h"
#include "i2c.h"
#include "neo6m.h"

using namespace arduino;

/************** TASK HANDLES **************/
TaskHandle_t blinkyHandle = NULL;
TaskHandle_t debugHandle = NULL;



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

// Use this for Debug Testing
void debugTask(void *pvParameters)
{
    const TickType_t xDelay = 500 / portTICK_PERIOD_MS;

    float longitude = 0;
    float latitude = 0;

    for (;;)
    {
        if (encodeGPS())
        {
            longitude =  getLongitude();
            latitude = getLatitude();
        }
        Serial.printf("Alt: %.2f ft, Long/Lat: (%.8f, %.8f), Time: \n", 
                      getAltitude_ft(), longitude, latitude);
        vTaskDelay(xDelay);
    }
};



void setup()
{
    printf("Program Started!");
    delay(3000);

    // PIN ASSIGNMENT
    Serial.begin(USB_BAUD_RATE);
    pinMode(LED_OUTPUT_PIN, OUTPUT);
    pinMode(BUZZ_PIN, OUTPUT);

    // I/O INIT
    initI2C();

    // DEVICE INIT
    initBMP();
    initNEO6M();
    // initMPU6050();

    // TASK CREATION
    xTaskCreate(blinkyTask, "Blinky Task", 4096, NULL, 1, &blinkyHandle);
    xTaskCreate(debugTask, "Debug Task", 4096, NULL, 1, &debugHandle);
}

void loop()
{
    vTaskStartScheduler();
    printf("Wtf just happened");                                    // Should never Happen
}

