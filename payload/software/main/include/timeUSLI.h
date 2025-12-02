/*****************************************
 * 
 * Timer Header File
 * 
 * Desc:
 * These functions are meant to take the
 * date time for time indexing 
 * 
 *****************************************/

#ifndef timeUSLI_h
#define timeUSLI_h

#include <TimeLib.h>

/******** SETTINGS ********/


/******** FUNCTIONS ********/
time_t getTeensyTime();
void timeSetup();

#endif // timeUSLI_h