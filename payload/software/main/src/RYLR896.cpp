/*****************************************
 * 
 * RYLR896 LoRa Telemetry Source File
 * 
 *****************************************/

#include "RYLR896.h"
#include "defines.h"
#include "arduino_freertos.h"

/******** VARIABLES ********/
// Do I wanna add a mutex?
//      Yes because Main will also transmit data 
// Remove Serial Use within Task?
//      Nah make new header for serial port 


/******** STATIC FUNCTIONS ********/
// 1 = success, 0 = failure
static bool sendCommand(String cmd, String expectedReply, int delay_ms)
{
    String cmdTemp = cmd + "\r\n";
    String reply = "";

    RYLR896_SERIAL.print(cmdTemp);
    vTaskDelay(delay_ms);                                               // Wait a bit for reply

    while (RYLR896_SERIAL.available())
    {
        reply = RYLR896_SERIAL.readStringUntil('\n');
        reply.replace("\n", "");
        reply.replace("\r", "");
    }

    // Debug
    //Serial.print(reply);
    //Serial.printf("\n");

    if (reply == expectedReply) 
    {
        return 1;
    }
    else 
    {
        Serial.printf("RYLR896 Failed Command at \"%s\" \n", cmd.c_str());
        return 0;
    }
        
}

/******** EXTERNAL FUNCTIONS ********/

void initRYLR896()
{
    RYLR896_SERIAL.begin(RYLR896_BAUD_RATE);

    sendCommand("AT+ADDRESS=12", "+OK", 500);
    sendCommand("AT+NETWORKID=4", "+OK", 500);
    sendCommand("AT+BAND=901000000", "+OK", 500);
    sendCommand("AT+PARAMETER=12,4,1,7", "+OK", 500);

    vTaskDelay(1000);                                               // RYLR896 Needs time to setup

    // Clear Input Buffer
    while (RYLR896_SERIAL.available())            
        RYLR896_SERIAL.read(); 
}       

// Send data via RYLR896, 1 if success, 0 if fail
bool txRYLR896(String data)
{
    // Send to Address 1
    char buf[128];
    snprintf(buf, sizeof(buf), "AT+SEND=1,%i,%s", data.length(), data.c_str());
    String out = buf;

    // Debug
    Serial.printf("Sending Message: ");
    Serial.println(out);

    return sendCommand(out, "+OK", 3000);
}         