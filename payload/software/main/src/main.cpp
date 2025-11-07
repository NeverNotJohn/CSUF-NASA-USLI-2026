#include "arduino_freertos.h"
#include "defines.h"
#include "motor.h"
#include "bmp280.h"
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

void testBMP(void *pvParameters){
    while (true)
    {
        Serial.print("Temperature: ");
        Serial.print(getTemperature());
        Serial.print(" *C ");
      
        Serial.print("Pressure: ");
        Serial.print(getPressure());
        Serial.print(" Pa ");
      
        Serial.print("Baseline Pressure: ");
        Serial.print(getBaselinePressure());
        Serial.print(" hPa ");

        // Serial.print("Altitude: ");
        // Serial.print(getAltitude());
        // Serial.print(" m");

        Serial.print("Relative Altitude: ");
        Serial.print(getRelativeAltitude());
        Serial.println(" m");
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
    xTaskCreate(testBMP, "BMP280 Test Task", 4096, NULL, 1, &bmpTestHandle);
    // xTaskCreate(testMPU, "MPU6050 Test Task", 4096, NULL, 1, &bmpTestHandle);

    vTaskStartScheduler();
}

void loop()
{

}

