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
    // Task Creation
    xTaskCreate(testTask, "Test Task", 1000, NULL, 1, &testTaskHandle);

    vTaskStartScheduler();
}

void loop()
{
    // Do Stuff
    cout << "I am alive! \n";
    myFunction(1, 2);
}

