/******************** DEFINITIONS GO HERE ********************/
#ifndef DEFINES_H
#define DEFINES_H

#include <Arduino.h>
#include <iostream>

/************** FLAGS **************/
#define ANNOY_CYAN          1
 
// Preflight
#define TRIGGER_FT          3       // 30

// Inflight
#define FLIGHT_TIMEOUT_S    900
#define GROUND_COUNTER_MAX  250

// Postflight
#define POST_FLIGHT_TIME_S  60      // 900 = 15 minutes

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