/*****************************************
 * 
 * Servo Control Header File
 * 
 * Servo control used to control the servo motor for flipping
 *****************************************/

#ifndef SERVOCONTROL_H
#define SERVOCONTROL_H
#include "Servo.h"
/************** DEFINES **************/
#define NUM_SERVOS 4


/************** CLASS DEFINITION **************/

class ServoControl {
private:
    Servo servo;
    int currentAngle;
    int pin;

public:
    ServoControl();
    ServoControl(int pin);  // init with pin

    void init(int pin);
    void setAngle(int angle);
    void turnToAngle(int targetAngle, int time_ms);
    void callibrate();

    int getAngle_deg() { return currentAngle; }
    int getPin()   { return pin; }
};


#endif // SERVOCONTROL_H