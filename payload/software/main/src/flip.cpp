/*****************************************
 * 
 * Flip Source File
 * 
 *****************************************/

#include <Arduino.h>
#include "servocontrol.h"
#include "flip.h"
/************** STATIC VARS **************/
ServoControl servos[NUM_SERVOS] = {};
#define SERVO_SPEED_MS 500
/************** FUNCTIONS **************/

void initLegs(int servoPins[NUM_SERVOS])
{
    Serial.println("initializing legs...");
    for (int i = 0; i < NUM_SERVOS; i++) {
      ServoControl servo = ServoControl();
      servos[i] = servo;
      servos[i].init(servoPins[i], 175); // attach servo to pin with 175 deg start angle
    }
    Serial.println("legs initialized!");    
    
}

void acuateLeg(int servoIndex, int targetAngle, int time_ms)
{

    servos[servoIndex].turnToAngle(targetAngle, time_ms);

}


void standUp()
{
    for (int i = 0; i < NUM_SERVOS; i++) {
                servos[i].prepareAngle(25, SERVO_SPEED_MS);
            }
            synchronizeAllServosStartAndWaitForAllServosToStop();
        //     delay(500);
        //     for (int i = 0; i < NUM_SERVOS; i++) {
        //         servos[i].prepareAngle(175, 1000);
        //     }
        // synchronizeAllServosStartAndWaitForAllServosToStop();
}

void standDown()
{
    for (int i = 0; i < NUM_SERVOS; i++) {
            servos[i].prepareAngle(175, SERVO_SPEED_MS);
    }
    synchronizeAllServosStartAndWaitForAllServosToStop();
    // for (int i = 0; i < NUM_SERVOS/2; i++) {
    //     servos[i].prepareAngle(0, SERVO_SPEED_MS);
    //     servos[i+2].prepareAngle(90, SERVO_SPEED_MS);

    // }
    // synchronizeAllServosStartAndWaitForAllServosToStop();
    // delay(500);
    // for (int i = 0; i < NUM_SERVOS; i++) {
    //     servos[i].prepareAngle(175, SERVO_SPEED_MS);
    // }
    // synchronizeAllServosStartAndWaitForAllServosToStop();

}