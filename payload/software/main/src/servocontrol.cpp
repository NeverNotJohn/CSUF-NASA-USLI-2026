/*****************************************
 * 
 * Servo Source File 
 * 
 *****************************************/
#include <Arduino.h>
#include <ServoEasing.hpp>
#include "servocontrol.h"

/******** PRIVATE VARS ********/

/******** INTERNAL FUNCTIONS ********/

// In ServoControl.h, add a method:


/******** EXTERNAL FUNCTIONS ********/

ServoControl::ServoControl() : pin(-1), currentAngle(175) {}

ServoControl::ServoControl(int pin, int startAngle) : pin(pin), currentAngle(startAngle) {
    init(pin, startAngle);
}

void ServoControl::prepareAngle(int targetAngle, int time_ms) {
    if (time_ms <= 0 || targetAngle == currentAngle) {
        setAngle(targetAngle);
        return;
    }
    currentAngle = targetAngle;
    servo.setEaseToD(targetAngle, time_ms); // takes ms directly, no conversion needed
    // servo.setEaseTo(targetAngle, (abs(targetAngle - servo.read()) / (float)time_ms) * 1000.0f);
}

void ServoControl::init(int pin, int startAngle) {
    this->pin    = pin;
    currentAngle = startAngle;

    // Attach at startAngle
    servo.attach(pin, startAngle);
    servo.setEasingType(EASE_LINEAR);
}

void ServoControl::setAngle(int angle) {
    if (currentAngle == angle) return;
    currentAngle = angle;
    servo.write(angle);  // direct write, no easing
}

void ServoControl::turnToAngle(int targetAngle, int time_ms) {
    if (time_ms <= 0 || targetAngle == currentAngle) {
        setAngle(targetAngle);
        return;
    }

    int angleDelta = abs(targetAngle - currentAngle);
    float speed = (angleDelta / (float)time_ms) * 1000.0f;

    Serial.println("Turning servo on pin " + String(pin) + " from " + String(currentAngle) 
        + "deg to " + String(targetAngle) + "deg in " + String(time_ms) + "ms (speed: " 
        + String(speed) + "deg/s)");
    
    currentAngle = targetAngle;
    servo.startEaseToD(targetAngle, time_ms); // takes ms directly, no conversion needed
}

void ServoControl::callibrate() {
    turnToAngle(180, 5000);
}

