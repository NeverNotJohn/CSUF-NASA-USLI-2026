/******************** DEFINITIONS GO HERE ********************/
#ifndef DEFINES_H
#define DEFINES_H

#include <Arduino.h>
#include <iostream>

/************** FLAGS **************/
#define ANNOY_CYAN          1
#define TRIGGER_FT          3 // 30
#define FLIGHT_TIMEOUT_S    900
#define GROUND_COUNTER_MAX  250

/************** PINS **************/
#define LED_OUTPUT_PIN      30
#define BUZZ_PIN            31 
#define LED_INTERNAL_PIN    13

/************** SETTINGS **************/
#define USB_BAUD_RATE       9600

/************** TYPEDEFS AND STRUCTS **************/
// typedef struct {
//     double x;
//     double y;
//     double z;
// } Vector3D;

typedef enum {
    PRE_FLIGHT,
    IN_FLIGHT,
    POST_FLIGHT,
    MISSION_END
} MissionState;

#endif // DEFINES_H