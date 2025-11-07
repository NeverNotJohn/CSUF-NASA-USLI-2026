#include "arduino_freertos.h"
#include "defines.h"
#include "motor.h"
#include "mpu6050.h"

using namespace std;

// Globals
// FIXME put somewhere else
TaskHandle_t testTaskHandle = NULL;
TaskHandle_t bmpTestHandle = NULL;

void testTask(void *pvParameters)
{
    while (true)
    {
        // printf("I am a task");
        myFunction(1,2);
        printf("\n");
    }
}


void testMPU(void *pvParameters)
{
    while (true)
    {

    }
}

void setup()
{
    Serial.begin(9600);
    // initBMP();
    initMPU6050();
    // Task Creation
    // xTaskCreate(testTask, "Test Task", 2048, NULL, 1, &testTaskHandle);
    xTaskCreate(testMPU, "MPU6050 Test Task", 4096, NULL, 1, &bmpTestHandle);

    vTaskStartScheduler();
}

void loop()
{

}

