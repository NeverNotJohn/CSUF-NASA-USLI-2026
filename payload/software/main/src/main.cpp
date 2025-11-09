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
Madgwick filter;

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
    TickType_t xLastWakeTime = xTaskGetTickCount(); // Get current time in ticks
    const TickType_t xFrequency = pdMS_TO_TICKS(10); // 100Hz/10ms
    const double deltaTime = pdTICKS_TO_MS(xFrequency)/1000.0; // auto converts ticks to seconds for delta time
    
    Vector3D orientation = {0, 0, 0}; // Init this out side of the loop
    // freeRTOS notated infinite loop 
    for(;;)
    {
        Vector3D angular = {0, 0, 0};
        Vector3D acceleration = {0, 0, 0};
        // orientationCalculation(angular,acceleration,orientation,deltaTime);
        // Convert the orientation into degrees btw
        // Serial.print("Orientation - ");
        // Serial.print("x (Roll): ");
        // Serial.print(orientation.x * 180.0 / PI);
        // Serial.print(" y (Pitch): ");
        // Serial.print(orientation.y * 180.0 / PI);
        // Serial.print(" z (Yaw): ");
        // Serial.println(orientation.z * 180.0 / PI);
        if(xSemaphoreTake(i2cMutex, portMAX_DELAY) == pdTRUE)
        {
            angular = getAngularVelocity();
            acceleration = getAcceleration();
            xSemaphoreGive(i2cMutex);
        }
        filter.updateIMU(angular.x,angular.y,angular.z,acceleration.x,acceleration.y,acceleration.z);

        float roll = filter.getRoll();
        float pitch = filter.getPitch();
        float heading = filter.getYaw();
        
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
    filter.begin(200);
    // setBeta was added in by me to test if changing the beta would make it more responsive
    //filter.setBeta(0.5f); 
    // Task Creation
    // xTaskCreate(testTask, "Test Task", 2048, NULL, 1, &testTaskHandle);
    // xTaskCreate(testMPU, "MPU6050 Test Task", 4096, NULL, 1, &bmpTestHandle);
    xTaskCreate(testBMP, "BMP280 Test Task", 4096, NULL, 1, &bmpTestHandle);
    xTaskCreate(orientationLoop, "getOrientation", 4096, NULL, 1, &orientationLoopHandle);

    vTaskStartScheduler();
}

void loop()
{

}

