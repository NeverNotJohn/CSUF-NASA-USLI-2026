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
#define SERVO_SPEED_MS 10000
/************** FUNCTIONS **************/

void initLegs(int servoPins[NUM_SERVOS])
{
    Serial.println("initializing legs...");
    for (int i = 0; i < NUM_SERVOS; i++) {
      ServoControl servo = ServoControl();
      servos[i] = servo;
      servos[i].init(servoPins[i], 175); // attach servo to pin with 175 deg start angle
    }
    delay(500);
    Serial.println("legs initialized!");    
    
}

void acuateLeg(int servoIndex, int targetAngle, int time_ms)
{

    servos[servoIndex].turnToAngle(targetAngle, time_ms);

}


void standUp()
{
    for (int i = 0; i < NUM_SERVOS; i++) {
                servos[i].prepareAngle(25, 1000);
            }
            synchronizeAllServosStartAndWaitForAllServosToStop();
            delay(500);
            for (int i = 0; i < NUM_SERVOS; i++) {
                servos[i].prepareAngle(175, 1000);
            }
        synchronizeAllServosStartAndWaitForAllServosToStop();
}

void standDown()
{
    for (int i = 0; i < NUM_SERVOS; i++) {
            servos[i].prepareAngle(175, 1000);
    }
    synchronizeAllServosStartAndWaitForAllServosToStop();
    delay(500);
    for (int i = 0; i < NUM_SERVOS/2; i++) {
        servos[i].prepareAngle(0, 1000);
        servos[i+2].prepareAngle(90, 1000);

    }
    synchronizeAllServosStartAndWaitForAllServosToStop();
    delay(500);
    for (int i = 0; i < NUM_SERVOS; i++) {
        servos[i].prepareAngle(175, 1000);
    }
    synchronizeAllServosStartAndWaitForAllServosToStop();

}