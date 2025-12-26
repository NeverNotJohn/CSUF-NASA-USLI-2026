/*****************************************
 * 
 * RYLR896 LoRa Telemetry Source File
 * 
 *****************************************/

#include "RYLR896.h"
#include "bmp280.h"
#include "defines.h"
#include <stdint.h>
#include "arduino_freertos.h"
#include "serialUSLI.h"
#include "beep.h"

/******** DEFINES ********/

typedef enum
{
    CMD_OK,
    CMD_CALIBRATE,
    CMD_ARM
} RxCommands;

/******** EXTERN VARS ********/
extern TaskHandle_t rxHandle = NULL;


/******** STATIC VARS ********/
SemaphoreHandle_t rylr896Mutex;
uint32_t rylr896Flags;

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

static bool sendCommandInput(String cmd)
{
    String cmdTemp = cmd + "\r\n";
    String reply = "";

    if (xSemaphoreTake(rylr896Mutex, portMAX_DELAY) == pdTRUE)
    {
        RYLR896_SERIAL_IN.print(cmdTemp);
    }

    vTaskDelay(500);                                                // Wait for +OK

    return 1;
}

uint32_t decodePacket(String line)
{
    uint32_t result = CMD_OK; 
    if (line == "+OK") return CMD_OK;
    
    // Decode Data Packet
    char data;
    char c;
    int counter = 0;
    for (int i = 0; i < line.length(); i++)
    {
        c = line[i];
        if (counter == 2)   
        {  
            data = c;
            break;
        }
        else if (c == ',') ++counter;
    }

    switch (data)
    {
        case 'C':
            result = CMD_CALIBRATE;
            break;
        case 'A':
            result = CMD_ARM;
            break;
    }
    return result;
}

/******** EXTERNAL FUNCTIONS ********/

void rxTask(void *pvParameters)
{
    String line;
    int cmd;
    for (;;)
    {
        if (RYLR896_SERIAL.available())
        {
            line = RYLR896_SERIAL.readStringUntil('\n');
            line.trim();
            
            cmd = decodePacket(line);

            switch (cmd)
            {
                case CMD_CALIBRATE:
                    Serial.println("Calibrating!");
                    calibrateBMP();
                    beep(3, 100, 50);
                    break;
                case CMD_ARM:
                    Serial.println("Arming!");
                    // Arming logic
                    beep(1, 3000, 0);
                    break;
            }

        }

        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

void initRYLR896()
{
    RYLR896_SERIAL.begin(RYLR896_BAUD_RATE);
    RYLR896_SERIAL_IN.begin(RYLR896_BAUD_RATE);
    rylr896Mutex = xSemaphoreCreateMutex();

    sendCommand("AT+ADDRESS=12");
    sendCommand("AT+NETWORKID=4");
    sendCommand("AT+BAND=901000000");
    sendCommand("AT+PARAMETER=12,4,1,7");

    sendCommandInput("AT+ADDRESS=13");
    sendCommandInput("AT+NETWORKID=4");
    sendCommandInput("AT+BAND=901000000");
    sendCommandInput("AT+PARAMETER=12,4,1,7");

    vTaskDelay(1000);                                               // RYLR896 Needs time to setup

    // Clear Input Buffer
    if (xSemaphoreTake(rylr896Mutex, portMAX_DELAY) == pdTRUE)
    {
        while (RYLR896_SERIAL.available())            
            RYLR896_SERIAL.read(); 
    
        xSemaphoreGive(rylr896Mutex);
    }

    xTaskCreate(rxTask, "RX Task", 4096, NULL, 1, &rxHandle);
}       

// Send data via RYLR896, 1 if success, 0 if fail
bool txRYLR896(const char* data)
{
    char buf[256];
    snprintf(buf, sizeof(buf), "AT+SEND=1,%i,%s", strlen(data), data);

    // Debug
    if (openUSB())
    {
        Serial.print("Sending Message: ");
        Serial.println(buf);
        closeUSB();
    }

    return sendCommand(buf);
}