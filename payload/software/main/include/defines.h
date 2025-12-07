/******************** DEFINITIONS GO HERE ********************/
#ifndef DEFINES_H
#define DEFINES_H

#include <Arduino.h>
#include <iostream>

using namespace std;

// typedefs & structs

typedef struct {
    float n;
    float hour;
    float min;
    float sec;
    float altitude_ft;
    float lng;
    float lat;
    float roll;
    float pitch;
    float yaw;
} DataPacket;

// typedef struct {
//     double x;
//     double y;
//     double z;
// } Vector3D;

#endif // DEFINES_H