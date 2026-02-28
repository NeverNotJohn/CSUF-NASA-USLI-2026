/*****************************************
 * 
 * Flip Source File
 * 
 *****************************************/

#include <arduino_freertos.h>
#include "servocontrol.h"
#include "flip.h"
/************** STATIC VARS **************/
ServoControl servos[NUM_SERVOS] = {};

/************** FUNCTIONS **************/

void initLegs(int servoPins[NUM_SERVOS])
{
    for (int i = 0; i < NUM_SERVOS; i++) {
        ServoControl servo;
        servo.init(servoPins[i]);
        servo.callibrate();
        servos[i] = servo;
    }
}

void acuateLeg(int servoIndex, int targetAngle, int time_ms)
{

    servos[servoIndex].turnToAngle(targetAngle, time_ms);

}


void standUp()
{
    for (int i = 0; i < NUM_SERVOS; i++) {
        servos[i].turnToAngle(90, 1000);
    }
}

void standDown()
{
    for (int i = 0; i < NUM_SERVOS; i++) {
        if(i < 2){
            servos[i].turnToAngle(90, 1000);
        } else {
            servos[i].turnToAngle(120, 1000);
        }
    }
}