#include "arduino_freertos.h"
#include "defines.h"
#include "motor.h"
#include "mpu6050.h"
#include "bmp280.h"
#include <MadgwickAHRS.h>
using namespace std;

// Globals
// FIXME put somewhere else
TaskHandle_t testTaskHandle = NULL;
TaskHandle_t bmpTestHandle = NULL;
TaskHandle_t orientationLoopHandle = NULL;
SemaphoreHandle_t i2cMutex = NULL;
Madgwick filterMadgwick;

void testTask(void *pvParameters)
{
    while (true)
    {
        // printf("I am  a task");
        // printf("I am a task");
        myFunction(1,2);
        printf("\n");
    }
}


void orientationLoop(void *pvParameters)
{
    TickType_t xLastWakeTime = xTaskGetTickCount(); // Get current time in ticks
    const TickType_t xFrequency = pdMS_TO_TICKS(10); // 100Hz/10ms
    const double deltaTime = pdTICKS_TO_MS(xFrequency)/1000.0; // auto converts ticks to seconds for delta time
        // freeRTOS notated infinite loop 
    for(;;)
    {
        Vector3D angular = {0, 0, 0};
        Vector3D acceleration = {0, 0, 0};
        if(xSemaphoreTake(i2cMutex, portMAX_DELAY) == pdTRUE)
        {
            angular = getAngularVelocity();
            acceleration = getAcceleration();
            xSemaphoreGive(i2cMutex);
        }
        filterMadgwick.updateIMU(angular.x,angular.y,angular.z,acceleration.x,acceleration.y,acceleration.z);

        float roll = filterMadgwick.getRoll();
        float pitch = filterMadgwick.getPitch();
        float heading = filterMadgwick.getYaw();
        
        Serial.printf("Roll: %.2f, Pitch: %.2f, Yaw: %.2f\n", 
            roll,  // Roll
            pitch,  // Pitch
            heading); // Yaw
        vTaskDelayUntil(&xLastWakeTime, xFrequency); // Consistently delay this task withouy yielding the CPU
    }
}
      
void testBMP(void *pvParameters)
{
    TickType_t xLastWakeTime = xTaskGetTickCount(); // Get current time in ticks
    const TickType_t xFrequency = pdMS_TO_TICKS(10); // 100Hz/10ms
    for(;;)
    {
        if(xSemaphoreTake(i2cMutex, portMAX_DELAY) == pdTRUE)
        {
            float temp = getTemperature();
            float pressure = getPressure();
            float baseline = getBaselinePressure();
            float altitude = getRelativeAltitude();
            xSemaphoreGive(i2cMutex);

            Serial.printf("Temperature: %f *C, Pressure: %f Pa, Baseline Pressure: %f hPa, Relative Altitude: %f m\n", 
                temp, pressure, baseline, altitude);
        }
        vTaskDelayUntil(&xLastWakeTime, xFrequency);
    }
}

void setup()
{
    Serial.begin(9600);
    i2cMutex = xSemaphoreCreateMutex();
    // Init sensors for tasks
    initBMP();
    initMPU6050();
    filterMadgwick.begin(100); // Begin Madgwick filter at 100Hz
    // Task Creation
    // xTaskCreate(testTask, "Test Task", 2048, NULL, 1, &testTaskHandle);
    xTaskCreate(testBMP, "BMP280 Test Task", 4096, NULL, 1, &bmpTestHandle);
    xTaskCreate(orientationLoop, "getOrientation", 4096, NULL, 1, &orientationLoopHandle);

    vTaskStartScheduler();
}

void loop()
{

}

