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
/**
 * @brief Calculates the orientation using angular velocity and acceleration over a set delta time (to be handled by the loop).
 * This function takes four parameters and calculates Roll, Pitch, and Yaw.
 * 
 * @param gyroData A Vector3D with angular velocity data
 * @param accelData A Vector3D with acceleration data
 * @param orientation Reference to a Vector3D that will be mutated in place
 * @param deltaTime Difference in time.
 * @param accelMix Weight for gyro vs accel (0.96-0.98 typical).
 */
void orientationCalculation(Vector3D gyroData, Vector3D accelData, Vector3D& orientation, double deltaTime, double accelMix = 0.98)
{
    // Integrate angular velocity to get orientation with respect to time
    // Very prone to drift but proof of concept!!

    orientation.x += gyroData.x * deltaTime; // Roll 
    orientation.y += gyroData.y * deltaTime; // Pitch
    orientation.z += gyroData.z * deltaTime; // Yaw
    
    // Calculate roll and pitch from the accelerometer
    float accelRoll = atan2(accelData.y, accelData.z);
    float accelPitch = atan2(-accelData.x, sqrt(accelData.y * accelData.y + accelData.z * accelData.z));
    
    // Apply complementary filter that blends in the gyro intergration with accel data
    // Greater accuracy for roll and pitch, sadly no yaw correction
    // Slowly will drift to the right orientation
    orientation.x  = accelMix * orientation.x + (1.0 - accelMix) * accelRoll;
    orientation.y = accelMix * orientation.y + (1.0 - accelMix) * accelPitch;
}

void orientationLoop(void *pvParameters)
{
    // freeRTOS notated infinite loop 
    TickType_t xLastWakeTime = xTaskGetTickCount(); // Get current time in ticks
    const TickType_t xFrequency = pdMS_TO_TICKS(10); // 100Hz/10MS
    const double deltaTime = 0.01; // 0.01s = 10ms
    Vector3D orientation = {0, 0, 0}; // Init this out side of the loop
    for(;;){
        Vector3D angular = getAngularVelocity();
        Vector3D acceleration = getAcceleration();
        orientationCalculation(angular,acceleration,orientation,deltaTime);
        // Convert the orientation into degrees btw
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

