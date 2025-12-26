/*****************************************
 * 
 * Beep Source File
 * 
 *****************************************/

#include "beep.h"
#include "arduino_freertos.h"
#include "defines.h"

/************** STATIC VARIABLES **************/
SemaphoreHandle_t buzzerMutex;

/************** EXTERN FUNCS **************/
void initBeep()
{
    buzzerMutex = xSemaphoreCreateMutex();
}

void beep(int numBeeps, double onInterval_ms, double offInterval_ms)
{

    if (!ANNOY_CYAN) return;

    if (xSemaphoreTake(buzzerMutex, portMAX_DELAY) == pdTRUE)
    {
        const TickType_t offInterval = offInterval_ms / portTICK_PERIOD_MS; 
        const TickType_t onInterval = onInterval_ms / portTICK_PERIOD_MS; 

        for (int i = 0; i < numBeeps; i++)
        {
            digitalWrite(BUZZ_PIN, 1);
            vTaskDelay(onInterval);
            digitalWrite(BUZZ_PIN, 0);
            vTaskDelay(offInterval);
        }

        xSemaphoreGive(buzzerMutex);
    }
}