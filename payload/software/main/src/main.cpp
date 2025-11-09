#include "arduino_freertos.h"
#include "defines.h"
#include "motor.h"
#include "mpu6050.h"

using namespace std;

// Globals
// FIXME put somewhere else
TaskHandle_t testTaskHandle = NULL;
TaskHandle_t bmpTestHandle = NULL;
TaskHandle_t orientationLoopHandle = NULL;

void testTask(void *pvParameters)
{
    while (true)
    {
        // printf("I am  a task");
        myFunction(1,2);
        printf("\n");
    }
}


void testMPU(void *pvParameters)
{
    while (true)
    {
        Vector3D accel = getAcceleration();
        Vector3D angular = getAngularVelocity();
        Serial.print("Acceleration - ");
        Serial.print("x: ");
        Serial.print(accel.x);
        Serial.print(" y: ");
        Serial.print(accel.y);
        Serial.print(" z: ");
        Serial.print(accel.z);
        Serial.print(" (m/s^2) ");
        Serial.print("Angular Acceleration - ");
        Serial.print("x: ");
        Serial.print(angular.x);
        Serial.print(" y: ");
        Serial.print(angular.y);
        Serial.print(" z: ");
        Serial.print(angular.z);
        Serial.println(" (rad/s)");
    }
}

void orientationLoop(void *pvParameters){
    // freeRTOS notated infinite loop 
    TickType_t xLastWakeTime = xTaskGetTickCount(); // Get current time in ticks
    const TickType_t xFrequency = pdMS_TO_TICKS(10); // 100Hz/10MS
    const double deltaTime = 0.01; // 0.01s = 10ms
    Vector3D orientation = {0, 0, 0};
    for(;;){
        Vector3D angular = getAngularVelocity();
        // Integrate angular velocity to get orientation with respect to time
        // Very prone to drift but proof of concept!!

        orientation.x += angular.x * deltaTime; // Roll 
        orientation.y += angular.y * deltaTime; // Pitch
        orientation.z += angular.z * deltaTime; // Yaw

        Serial.print("Orientation - ");
        Serial.print("x (Roll): ");
        Serial.print(orientation.x * 180.0 / PI);
        Serial.print(" y (Pitch): ");
        Serial.print(orientation.y * 180.0 / PI);
        Serial.print(" z (Yaw): ");
        Serial.println(orientation.z * 180.0 / PI);

        vTaskDelayUntil(&xLastWakeTime, xFrequency); // Consistently delay this task withouy yielding the CPU
    }
}

void setup()
{
    Serial.begin(9600);
    // Init sensors for tasks
    // initBMP();
    initMPU6050();
    // Task Creation
    // xTaskCreate(testTask, "Test Task", 2048, NULL, 1, &testTaskHandle);
    // xTaskCreate(testMPU, "MPU6050 Test Task", 4096, NULL, 1, &bmpTestHandle);
    xTaskCreate(orientationLoop, "getOrientation", 4096, NULL, 1, &orientationLoopHandle);

    vTaskStartScheduler();
}

void loop()
{

}

