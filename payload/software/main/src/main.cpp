#include "arduino_freertos.h"
#include "defines.h"
#include "motor.h"
#include "mpu6050.h"
#include "bmp280.h"
#include "RYLR896.h"
#include "i2c.h"
#include "neo6m.h"
#include <time.h>
#include "timeUSLI.h"

using namespace arduino;

/************** TASK HANDLES **************/
TaskHandle_t blinkyHandle = NULL;
TaskHandle_t loggerHandle = NULL;
TaskHandle_t mainHandle = NULL;

/************** GLOBAL VARS **************/
MissionState currentState;
SemaphoreHandle_t buzzerMutex;
// FIXME Mutex for buzzer?

/************** HELPER FUNCTIONS **************/
// Beeps
void beep(int numBeeps, double onInterval_ms, double offInterval_ms)
{

    if (!ANNOY_CYAN) return;

    if (xSemaphoreTake(buzzerMutex, portMAX_DELAY) == pdTRUE)
    {
        const TickType_t offInterval = offInterval_ms / portTICK_PERIOD_MS; 
        const TickType_t onInterval = onInterval_ms / portTICK_PERIOD_MS; 

        for (int i = 0; i < numBeeps; i++)
        {
            digitalWrite(BUZZ_PIN, 1);
            vTaskDelay(onInterval);
            digitalWrite(BUZZ_PIN, 0);
            vTaskDelay(offInterval);
        }

        xSemaphoreGive(buzzerMutex);
    }
}



/************** THREADS **************/
// Sanity Check
void blinkyTask(void *pvParameters)
{
    // Block for 5000 ms
    const TickType_t xDelay = 5000 / portTICK_PERIOD_MS; 
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
void loggerTask(void *pvParameters)
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
        Serial.printf("Alt: %.2f ft, Long/Lat: (%.8f, %.8f), Time: (%i:%i:%i) \n", 
                      getAltitude_ft(), longitude, latitude, hour(), minute(), second());
        vTaskDelay(xDelay);
    }
};

// Main Task
void mainTask(void *pvParameters)
{
    beep(3, 100, 50);
    currentState = PRE_FLIGHT;
    int preFlightCounter = 0;

    // Preflight
    while (preFlightCounter < 10)
    {

        // Look for calibration sequence

        if (getAltitude_ft() > TRIGGER_FT)
            preFlightCounter++;
        else
            preFlightCounter = 0;

        vTaskDelay(pdMS_TO_TICKS(100));
    }

    // Change States
    currentState = IN_FLIGHT;
    time_t liftOffTime_s = Teensy3Clock.get();
    int groundCounter = 0;

    // Outputs
    beep(1, 1000, 0);
    Serial.printf("LIFTOFF!\n");

    // Inflight
    // Triggers 25 secs after trigger
    while (groundCounter < GROUND_COUNTER_MAX && 
          ((Teensy3Clock.get() - liftOffTime_s) < FLIGHT_TIMEOUT_S))
    {
        if (getAltitude_ft() < TRIGGER_FT) 
            groundCounter++;
        else
            groundCounter = 0;

        vTaskDelay(pdMS_TO_TICKS(100));
    }

    // Change States
    currentState = POST_FLIGHT;
    time_t touchDownTime_s = Teensy3Clock.get();

    // Output
    beep(5, 100, 10);
    Serial.printf("TOUCHDOWN!\n");

    // Post flight
    // Cool Rover Stuff
    while ((Teensy3Clock.get() - touchDownTime_s) < POST_FLIGHT_TIME_S)
    {
        // Do Rover Stuff
        vTaskDelay(pdMS_TO_TICKS(100));
    }

    // Change States
    currentState = MISSION_END;
    time_t missionEndTime_s = Teensy3Clock.get();

    // Output
    beep(3, 1000, 500);
    Serial.printf("YATA PLS SOIL!\n");

    // End of Mission
    for(;;) {}
}

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
    buzzerMutex = xSemaphoreCreateMutex();

    // DEVICE INIT
    initBMP();
    initNEO6M();
    setSyncProvider(getTeensyTime);
    // initMPU6050();
    Serial.printf("Init Finished! Time: %04d-%02d-%02d %02d:%02d:%02d\n",
                  year(), month(), day(), hour(), minute(), second());

    // TASK CREATION
    xTaskCreate(blinkyTask, "Blinky Task", 4096, NULL, 1, &blinkyHandle);
    xTaskCreate(mainTask, "Main Task", 4096, NULL, 1, &mainHandle);
    xTaskCreate(loggerTask, "Debug Task", 4096, NULL, 1, &loggerHandle);
}

void loop()
{
    vTaskStartScheduler();
    printf("Wtf just happened");                                    // Should never Happen
}

