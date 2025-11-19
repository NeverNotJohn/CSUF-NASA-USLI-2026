/******************** DEFINITIONS GO HERE ********************/
#ifndef DEFINES_H
#define DEFINES_H

#include <Arduino.h>
#include <iostream>
#include <stdint.h>
#include <stdlib.h>

/************** FLAGS **************/
#define ANNOY_CYAN          0


/************** PINS **************/
#define LED_OUTPUT_PIN      25
#define BUZZ_PIN            31 
#define LED_INTERNAL_PIN    13

/************** SETTINGS **************/
#define USB_BAUD_RATE       9600
#define DATA_BUFFER_SIZE    10416

/************** TYPEDEFS **************/
// typedef struct {
//     double x;
//     double y;
//     double z;
// } Vector3D;

typedef struct {
    uint32_t index;
    uint8_t hour;
    uint8_t min;
    uint8_t sec;
    float altitude;
    float latitude;
    float longitude;
    float roll;
    float pitch;
    float yaw;
    float pH;
    float EC;
} DataPacket;

#endif // DEFINES_H