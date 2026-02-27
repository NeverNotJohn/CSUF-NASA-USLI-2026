/*****************************************
 * 
 * Flipping Header File
 * 
 * for flippl
 *****************************************/

#ifndef FLIP_H
#define FLIP_H
#include "Servo.h"
/************** DEFINES **************/
#define NUM_SERVOS 4
ServoControl servos[NUM_SERVOS] = {};

/************** DEFINITION **************/
void initLegs(int servoPins[NUM_SERVOS]);
void standUp();
void standDown();
void acuateLeg();

#endif // FLIP_H