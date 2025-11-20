/*****************************************
 * 
 * NEO6M GPS Source File
 * 
 *****************************************/

#include "neo6m.h"
#include "defines.h"
#include <TinyGPS++.h>
TinyGPSPlus gps;
using namespace std;
void initNEO6M(){
    Serial1.begin(9600);
    Serial.print("neo6m data should be spittin");
}

bool encodeGPS(){
    bool updated = false;
    if (Serial1.available() > 0) {
        char c = Serial1.read();
        if (gps.encode(c)) {
            updated = true;
        }
    }
    return updated;
}

float getLongitude(){
    return gps.location.lng();
}

float getLatitude(){
    return gps.location.lat();
}

float getSats(){
    return gps.satellites.value();
}

void getGPSTimeString(){
    char buffer[9];
    sprintf(buffer, "%02d:%02d:%02d", gps.time.hour(), gps.time.minute(), gps.time.second());
    Serial.print(buffer);
}
