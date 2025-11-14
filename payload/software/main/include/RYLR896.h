/*****************************************
 * 
 * MPU6050 IMU Header File
 * 
 *****************************************/
#ifndef RYLR896_H
#define RYLR896_H

/******** SETTINGS ********/
#define RYLR896_BAUD_RATE   115200
#define RYLR896_SERIAL      Serial1             // Use Serial1 for RYLR896 communication
#define RYLR896_ADDRESS     0

/******** FUNCTIONS ********/
void initRYLR896();                             // Initialize RYLR896 module
bool txRYLR896(const char* data);               // Send data via RYLR896, 1 if success, 0 if fail

#endif // RYLR896_H