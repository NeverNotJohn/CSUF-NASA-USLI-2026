/*****************************************
 * 
 * Flipping Header File
 * 
 * for flippl
 *****************************************/

#ifndef FLIP_H
#define FLIP_H
#include "Servo.h"
#include "servocontrol.h"
/************** DEFINES **************/
#define NUM_SERVOS 4
/************** DEFINITION **************/
void initLegs(int servoPins[NUM_SERVOS]);
void standUp();
void standDown();
void acuateLeg(int servoIndex, int targetAngle, int time_ms);

#endif // FLIP_H