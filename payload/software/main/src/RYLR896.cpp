/*****************************************
 * 
 * RYLR896 LoRa Telemetry Source File
 * 
 *****************************************/

#include "RYLR896.h"
#include "defines.h"
#include "arduino_freertos.h"
#include "serialUSLI.h"

// Do I wanna add a mutex?
//      Yes because Main will also transmit data 
// Remove Serial Use within Task?
//      Nah make new header for serial port 

/******** STATIC VARS ********/
SemaphoreHandle_t rylr896Mutex;

/******** STATIC FUNCTIONS ********/
// 1 = success, 0 = failure
// FIXME
static bool sendCommand(String cmd)
{
    String cmdTemp = cmd + "\r\n";
    String reply = "";

    if (xSemaphoreTake(rylr896Mutex, portMAX_DELAY) == pdTRUE)
    {
        RYLR896_SERIAL.print(cmdTemp);
        xSemaphoreGive(rylr896Mutex);
    }

    vTaskDelay(500);                                                // Wait for +OK

    return 1;
}

/******** EXTERNAL FUNCTIONS ********/

void initRYLR896()
{
    RYLR896_SERIAL.begin(RYLR896_BAUD_RATE);
    rylr896Mutex = xSemaphoreCreateMutex();

    sendCommand("AT+ADDRESS=12");
    sendCommand("AT+NETWORKID=4");
    sendCommand("AT+BAND=901000000");
    sendCommand("AT+PARAMETER=12,4,1,7");

    vTaskDelay(1000);                                               // RYLR896 Needs time to setup

    // Clear Input Buffer
    if (xSemaphoreTake(rylr896Mutex, portMAX_DELAY) == pdTRUE)
    {
        while (RYLR896_SERIAL.available())            
            RYLR896_SERIAL.read(); 
    
        xSemaphoreGive(rylr896Mutex);
    }
}       

// Send data via RYLR896, 1 if success, 0 if fail
bool txRYLR896(String data)
{
    // Send to Address 1
    char buf[128];
    snprintf(buf, sizeof(buf), "AT+SEND=1,%i,%s", data.length(), data.c_str());
    String out = buf;

    // Debug
    if (openUSB())
    {
        Serial.printf("Sending Message: ");
        Serial.println(out);
        closeUSB();
    }

    return sendCommand(out);
}         