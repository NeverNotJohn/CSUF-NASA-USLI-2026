#include "arduino_freertos.h"
#include "defines.h"
#include "motor.h"

using namespace std;

// Globals
// FIXME put somewhere else
TaskHandle_t testTaskHandle = NULL;

void testTask(void *pvParameters)
{
    while (1)
    {
        printf("I am a task");
    }
}

void setup()
{
    Serial.begin(9600);
    // Task Creation
    xTaskCreate(testTask, "Test Task", 1000, NULL, 1, &testTaskHandle);

    vTaskStartScheduler();
}

void loop()
{
    // Do Stuff
    cout << "I am alive! \n";
    Serial.printf("Hello World \n");
    printf("Hello World");
    myFunction(1, 2);
}

