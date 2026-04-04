/*****************************************
 * 
 * BTS7960 Motor Driver Implementation
 * Chassis Control
 * 
 *****************************************/

#include "chassis.h"
#include <Arduino.h>

/************** VARIABLES **************/
static int currentSpeed = 0;
volatile long encoderCount = 0;

static unsigned long lastStallCheck = 0;
static uint32_t thresholdSince = 0; 
static bool overThreshold = false;

/************** FUNCTIONS **************/


void encoderISR() {
    bool b = digitalRead(CHASSIS_ENC_B);
    encoderCount += (b ? 1 : -1);
}

void initChassis() {
    pinMode(CHASSIS_RPWM_PIN, OUTPUT);
    pinMode(CHASSIS_LPWM_PIN, OUTPUT);
    pinMode(CHASSIS_R_IS_PIN, INPUT); //put 2.2kohm resistors, on both, for current sensing
    pinMode(CHASSIS_L_IS_PIN, INPUT);
    pinMode(CHASSIS_ENC_A, INPUT_PULLUP);
    pinMode(CHASSIS_ENC_B, INPUT_PULLUP);
    // six pins of doom
    analogWrite(CHASSIS_RPWM_PIN, 0);
    analogWrite(CHASSIS_LPWM_PIN, 0);
    attachInterrupt(digitalPinToInterrupt(CHASSIS_ENC_A), encoderISR, RISING);

    currentSpeed = 0;
    Serial.println("Chassis Initialized!");
}

void chassisRaise(int speed) {
    speed = constrain(speed, 0, MAX_SPEED);
    analogWrite(CHASSIS_LPWM_PIN, 0);
    analogWrite(CHASSIS_RPWM_PIN, speed);
    currentSpeed = speed;
}

void chassisLower(int speed) {
    speed = constrain(speed, 0, MAX_SPEED);
    analogWrite(CHASSIS_RPWM_PIN, 0);
    analogWrite(CHASSIS_LPWM_PIN, speed);
    currentSpeed = -speed;
}

void chassisStop() {
    analogWrite(CHASSIS_RPWM_PIN, 0);
    analogWrite(CHASSIS_LPWM_PIN, 0);
    currentSpeed = 0;
}

void chassisSetSpeed_pwm(int speed) {
    speed = constrain(speed, -MAX_SPEED, MAX_SPEED);
    if (speed > 0) {
        chassisRaise(speed);
    } else if (speed < 0) {
        chassisLower(-speed);
    } else {
        chassisStop();
    }
}


bool chassisStallDetected() {
    uint32_t now = millis();

    if (now - lastStallCheck < STALL_COOLDOWN_MS) {
        return false;
    }
    lastStallCheck = now;

    // read whichever IS pin corresponds to the active drive direction
    int sense = (currentSpeed >= 0) ? analogRead(CHASSIS_R_IS_PIN) : analogRead(CHASSIS_L_IS_PIN);
    if (sense > STALL_THRESHOLD) {
        if (!overThreshold) {
            overThreshold   = true;
            thresholdSince  = now;
        }
        // only declare stall after being over threshold continuously
        if (now - thresholdSince >= STALL_CONFIRM_MS) {
            return true;
        }
    } else {
        // dropped back below threshold, reset confirmation window
        overThreshold = false;
        thresholdSince = 0;
    }

    return false;
}

void calibrateChassis() {
    // Start moving inward slowly
    chassisRaise(HOME_SPEED);
    
    // Wait for stall detection 
    while (!chassisStallDetected()) {
        delay(10);
    }

    // Stop and set home position
    chassisStop();
    delay(100); 
    encoderCount = 0;
    chassisLower(HOME_SPEED);
    long targetCount = BACKOFF_COUNTS; 
    while (encoderCount > -targetCount) {
        delay(10);
    }
    chassisStop();
    encoderCount = 0;
    Serial.println("Chassis Homed!");
}