/*****************************************
 * 
 * NEO6M GPS Source File
 * 
 *****************************************/

#include "neo6m.h"
#include "defines.h"
#include <TinyGPS++.h>

/************** STATIC VARS **************/
TinyGPSPlus gps;

/************** FUNCTIONS **************/
void initNEO6M()
{
    NEO6M_SERIAL.begin(NEO6M_BAUD_RATE);
    Serial.print("neo6m data should be spittin");
}

bool encodeGPS()
{
    bool updated = false;
    if (NEO6M_SERIAL.available() > 0) {
        char c = NEO6M_SERIAL.read();
        if (gps.encode(c)) {
            updated = true;
        }
    }
    return updated;
}

float getLongitude()
{
    return gps.location.lng();
}

float getLatitude()
{
    return gps.location.lat();
}

float getSats()
{
    return gps.satellites.value();
}

void getGPSTimeString()
{
    char buffer[9];
    sprintf(buffer, "%02d:%02d:%02d", gps.time.hour(), gps.time.minute(), gps.time.second());
    Serial.print(buffer);
}
