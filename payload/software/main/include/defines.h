/******************** DEFINITIONS GO HERE ********************/
#ifndef DEFINES_H
#define DEFINES_H

#include <Arduino.h>
#include <iostream>
#include <stdint.h>

/************** FLAGS **************/
#define ANNOY_CYAN          1
 
// Preflight
#define TRIGGER_FT          5       // OFFICIAL: 30

// Inflight
#define FLIGHT_TIMEOUT_S    900
#define GROUND_COUNTER_MAX  250

// Postflight
#define POST_FLIGHT_TIME_S  900      // OFFICIAL: 900 = 15 minutes

/************** PINS **************/
#define LED_OUTPUT_PIN      13      // 28 is fried lol
#define TD3_PIN             10
#define BUZZ_PIN            12 
#define LED_INTERNAL_PIN    13

/************** SETTINGS **************/
#define USB_BAUD_RATE       9600
#define DATA_LOG_SIZE       10416
#define LOGGER_PERIOD_MS    500

// typedefs & structs

typedef struct {
    uint32_t n;
    uint8_t hour;
    uint8_t min;
    uint8_t sec;
    float altitude_ft;
    float lng;
    float lat;
    float roll_deg;
    float pitch_deg;
    float yaw_deg;
} DataPacket;

typedef struct {
    DataPacket data[DATA_LOG_SIZE];
    int size;
} DataPacketLog;

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