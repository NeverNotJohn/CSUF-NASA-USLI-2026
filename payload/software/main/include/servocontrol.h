/*****************************************
 * 
 * Servo Control Header File
 * 
 * Servo control used to control the servo motor for flipping
 *****************************************/
#ifndef SERVOCONTROL_H
#define SERVOCONTROL_H

#include <ServoEasing.h>
/************** DEFINES **************/
#define NUM_SERVOS 4

/************** CLASS DEFINITION **************/
class ServoControl {
private:
    ServoEasing servo;
    int currentAngle;
    int pin;

public:
    ServoControl();
    ServoControl(int pin, int startAngle = 180);    // init with pin and optional start angle
    void init(int pin, int startAngle = 180);
    void setAngle(int angle);
    void turnToAngle(int targetAngle, int time_ms); // non-blocking, call updateAllServos() in loop()
    void callibrate();
    void prepareAngle(int targetAngle, int time_ms);

    int getAngle_deg() { return currentAngle; }
    int getPin()       { return pin; }
    bool isMoving()    { return servo.isMoving(); }
};

#endif // SERVOCONTROL_H