#include "arduino_freertos.h"
#include "defines.h"
#include "motor.h"
#include "mpu6050.h"
#include "bmp280.h"
#include "sdfs.h"
#include "SD.h"
#include <vector>

using namespace std;

// Globals
// FIXME put somewhere else
TaskHandle_t testTaskHandle = NULL;
TaskHandle_t bmpTestHandle = NULL;
TaskHandle_t orientationLoopHandle = NULL;


void sdTask(void *parameter) {
    const char* filePath = (char*)parameter;
    createFile(filePath);
    const std::vector<DataPacket> dataArray = {
        {0, 12, 1, 1, 57.1, 11.0, 11.0, 11.0, 0.12, 0.125},
        {1, 12, 1, 2, 57.1, 11.0, 11.0, 11.0, 0.12, 0.125},
        {2, 12, 1, 2, 57.1, 11.0, 11.0, 11.0, 0.12, 0.125}
    };
    writeData(filePath,dataArray);
    vTaskDelete(NULL);
}


void setup()
{
    Serial.begin(9600);


    // Init sensors for tasks
    // initBMP();
    // initMPU6050();
    // Task Creation
    // xTaskCreate(testTask, "Test Task", 2048, NULL, 1, &testTaskHandle);
    // xTaskCreate(testMPU, "MPU6050 Test Task", 4096, NULL, 1, &bmpTestHandle);
    // createFile("/test.csv");
    // const std::vector<DataPacket> dataArray = {
    //     {0, 12, 1, 1, 57.1, 11.0, 11.0, 11.0, 0.12, 0.125},
    //     {1, 12, 1, 2, 57.1, 11.0, 11.0, 11.0, 0.12, 0.125},
    //     {2, 12, 1, 2, 57.1, 11.0, 11.0, 11.0, 0.12, 0.125}
    // };
    // writeData("/test.csv",dataArray);
    // xTaskCreate(orientationLoop, "getOrientation", 4096, NULL, 1, &orientationLoopHandle);
    // initBMP();
    // Task Creation
    xTaskCreate(sdTask, "Test Task", 2048, (void*)"/test_log.csv", 1, &testTaskHandle);
    // xTaskCreate(test, "BMP280 Test Task", 4096, NULL, 1, &bmpTestHandle);

    vTaskStartScheduler();
}

void loop()
{

}

