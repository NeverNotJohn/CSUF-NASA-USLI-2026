/*****************************************
 * 
 * RYLR896 LoRa Transceiver Header File
 * 
 *****************************************/
#ifndef RYLR896_H
#define RYLR896_H

#include "arduino_freertos.h"

/******** SETTINGS ********/
#define RYLR896_BAUD_RATE   115200
#define RYLR896_SERIAL      Serial1             // Use Serial1 for RYLR896 communication
#define RYLR896_ADDRESS     12

#define RYLR896_SERIAL_IN   Serial5
#define RYLR896_ADDRESS_IN  13

/******** FUNCTIONS ********/
void initRYLR896();                             // Initialize RYLR896 module
bool txRYLR896(const char *data);                    // Send data via RYLR896, 1 if success, 0 if fail

/******** EXTERN VARS ********/
extern TaskHandle_t rxHandle;

#endif // RYLR896_H