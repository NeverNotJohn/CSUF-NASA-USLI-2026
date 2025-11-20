#include "arduino_freertos.h"
#include "defines.h"
#include "motor.h"
#include "neo6m.h"
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
        // printf("I am a task");
        myFunction(1,2);
        printf("\n");
    }
}


void testBMP(void *pvParameters)
{
    for(;;) {
        if(encodeGPS()){
            Serial.print("Long=");
            Serial.print(getLongitude(),8);
            Serial.print(" Lat=");
            Serial.print(getLatitude(),8);
            Serial.print(" Time=");
            getGPSTimeString();
            Serial.print(" Sats=");
            Serial.println(getSats(),8);  
        } 
    }
};

void setup()
{
    Serial.begin(9600);
    initNEO6M();
    xTaskCreate(testBMP, "BMP280 Test Task", 4096, NULL, 1, &bmpTestHandle);

    vTaskStartScheduler();
}

void loop()
{

}

