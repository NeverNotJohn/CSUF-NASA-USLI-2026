/*****************************************
 * 
 * Servo Source File 
 * 
 * 
 *****************************************/

#include "defines.h"
#include "arduino_freertos.h"
#include "Servo.h"
#include "servocontrol.h"

/******** PRIVATE VARS ********/

/******** INTERNAL FUNCTIONS ********/

/******** EXTERNAL FUNCTIONS ********/
ServoControl::ServoControl() : pin(-1), currentAngle(90) {}

void ServoControl::init(int pin) {
    this->pin = pin;
    servo.attach(pin);
    servo.write(currentAngle);
}

void ServoControl::setAngle(int angle) {
    if (currentAngle == angle) return;
    currentAngle = angle;
    servo.write(angle);
}

void ServoControl::turnToAngle(int targetAngle, int time_ms) {
    // 20ms is a standard refresh rate servos.
    int stepInterval = 20; 
    int totalSteps = time_ms / stepInterval;

    if (totalSteps <= 0) {
        setAngle(targetAngle);
        return;
    }

    float angleIncrement = (float)(targetAngle - currentAngle) / totalSteps;
    float startAngle = (float)currentAngle;

    for (int i = 1; i <= totalSteps; i++) {
        int nextPos = startAngle + (angleIncrement * i);
        setAngle(nextPos);
        vTaskDelay(pdMS_TO_TICKS(stepInterval)); 
    }

    setAngle(targetAngle);
}

void ServoControl::callibrate() {
    setAngle(0);
}