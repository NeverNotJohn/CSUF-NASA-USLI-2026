#include "arduino_freertos.h"
#include "defines.h"
#include "motor.h"
#include "mpu6050.h"
#include "bmp280.h"
#include "RYLR896.h"
#include "i2c.h"
#include "neo6m.h"
#include "logger.h"
#include <time.h>
#include "timeUSLI.h"
#include "serialUSLI.h"
#include "beep.h"
#include "sdfs.h"
using namespace arduino;

/************** TASK HANDLES **************/
TaskHandle_t blinkyHandle = NULL;
TaskHandle_t mainHandle = NULL;
TaskHandle_t filterMPUHandle = NULL;
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

// Main Task
void mainTask(void *pvParameters)
{
    // Setup
    beep(3, 100, 50);
    setMissionState(PRE_FLIGHT);
    int preFlightCounter = 0;

    char filePath[256] = "2025-12-7_12-11-10_telemetry.csv";
    snprintf(filePath, sizeof(filePath), "%04d-%02d-%02d_%02d-%02d-%02d_telemetry.csv", 
            year(), month(), day(), hour(), minute(), second());

    createFile(filePath);

    // Preflight
    while (preFlightCounter < 30 || !(checkArmFlag()))
    {

        // Look for calibration sequence

        if (getAltitude_ft() > TRIGGER_FT)
            preFlightCounter++;
        else
            preFlightCounter = 0;

        vTaskDelay(pdMS_TO_TICKS(100));
    }

    // Change States
    setMissionState(IN_FLIGHT);
    time_t liftOffTime_s = Teensy3Clock.get();
    int groundCounter = 0;

    // Outputs
    beep(1, 1000, 500);

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
    setMissionState(POST_FLIGHT);
    time_t touchDownTime_s = Teensy3Clock.get();

    // Output
    beep(2, 1000, 500);

    // Release TD3
    digitalWrite(TD3_PIN, 1);

    // Post flight
    // Cool Rover Stuff
    while ((Teensy3Clock.get() - touchDownTime_s) < POST_FLIGHT_TIME_S)
    {
        // Do Rover Stuff
        vTaskDelay(pdMS_TO_TICKS(100));
    }

    // Change States
    setMissionState(MISSION_END);
    time_t missionEndTime_s = Teensy3Clock.get();

    // Output
    beep(3, 1000, 500);

    // Store Data Onto SD Card
    writeData(filePath, dataLog);

    // End of Mission
    for(;;) {}
}


void filterMPUTask(void *pvParameters){
    for(;;){
        updateMPUFilter();
        Serial.print("Roll: ");
        Serial.print(getRoll());
        Serial.print(" Pitch: ");
        Serial.print(getPitch());
        Serial.print(" Yaw: ");
        Serial.println(getYaw());
        vTaskDelay(pdMS_TO_TICKS(5));
    }
}

void setup()
{
    printf("Program Started!");
    delay(3000);

    // PIN ASSIGNMENT
    Serial.begin(USB_BAUD_RATE);
    pinMode(LED_OUTPUT_PIN, OUTPUT);
    pinMode(TD3_PIN, OUTPUT);
    pinMode(BUZZ_PIN, OUTPUT);

    // I/O INIT
    initI2C();
    initUSB();
    initBeep();
    initLogger();
    // timeSetup();

    // DEVICE INIT
    initBMP();
    initNEO6M();
    setSyncProvider(getTeensyTime);
    initMPU6050();
    Serial.printf("Init Finished! Time: %04d-%02d-%02d %02d:%02d:%02d\n",
                  year(), month(), day(), hour(), minute(), second());

    // TASK CREATION
    xTaskCreate(blinkyTask, "Blinky Task", 4096, NULL, 1, &blinkyHandle);
    //xTaskCreate(filterMPUTask, "MPU Task", 4096, NULL, 1, &filterMPUHandle);
    xTaskCreate(mainTask, "Main Task", 4096, NULL, 1, &mainHandle);
    xTaskCreate(loggerTask, "Debug Task", 4096, NULL, 1, &loggerHandle);
}

void loop()
{
    vTaskStartScheduler();
    printf("Wtf just happened");                                    // Should never Happen
}

