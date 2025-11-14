/*****************************************
 * 
 * RYLR896 LoRa Telemetry Source File
 * 
 *****************************************/

#include "RYLR896.h"
#include "defines.h"

/******** STATIC FUNCTIONS ********/

// NOT THREAD SAFE (yet)
// 1 = success, 0 = failure
static bool sendCommand(String cmd, String expectedReply)
{
    String cmdTemp = cmd + "\r\n";
    RYLR896_SERIAL.print(cmdTemp);
    delay(50);          // Wait a bit to send

    while (Serial1.available())
    {
        char c = RYLR896_SERIAL.read();
        Serial.printf("%c\n", c);
    }

    return false;
}

static bool testRYLR896()
{
    Serial.println("Hello World!");
    for (;;)
    {
        // put your main code here, to run repeatedly:
        while (RYLR896_SERIAL.available()) {
            Serial.write(RYLR896_SERIAL.read());
        }

        // Read from Serial Monitor and send to LoRa module
        while (Serial.available()) {
            char c = Serial.read();
            Serial.write(c);
            RYLR896_SERIAL.write(c);
        }
        delay(100);
    }
}

/******** EXTERNAL FUNCTIONS ********/

void initRYLR896()
{
    RYLR896_SERIAL.begin(RYLR896_BAUD_RATE);
    delay(1000);         // Wait a bit to init... blocking statement

    // Debug
    testRYLR896();
}

// Send data via RYLR896, 1 if success, 0 if fail
bool txRYLR896(const char* data)
{
    // Lorem Ipsum
}         