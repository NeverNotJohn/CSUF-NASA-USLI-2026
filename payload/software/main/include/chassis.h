/*****************************************
 * 
 * BTS7960 Motor Driver Header File
 * Chassis Control
 * 
 *****************************************/
#ifndef CHASSIS_H
#define CHASSIS_H

/************** DEFINES **************/
#define CHASSIS_RPWM_PIN        6
#define CHASSIS_LPWM_PIN        7
#define CHASSIS_R_IS_PIN        8
#define CHASSIS_L_IS_PIN        9

#define CHASSIS_ENC_A           8
#define CHASSIS_ENC_B           9 

#define MAX_SPEED               255
#define STALL_SPEED_THRESHOLD   20

// Tuned for this motor (12-bit ADC, 2.2kohm sense resistor)
#define STALL_THRESHOLD         1300  // between 218 (efficiency) and 1765 (stall)
#define STALL_CONFIRM_MS        5   
#define STALL_COOLDOWN_MS       100  
#define HOME_SPEED              60   // slow for accuracy, 0–255
#define BACKOFF_COUNTS          500  // back off after homing

/************** EXTERNAL FUNCTIONS **************/
// Init
void initChassis();

// Raise Chassis
void chassisRaise(int speed = 255);

// Lower Chassis
void chassisLower(int speed = 255);

// Stop Chassis
void chassisStop();

// Set chassis speed
void chassisSetSpeed_pwm(int speed);

// Detect motor stall
bool chassisStallDetected();

// Calibrate chassis
void calibrateChassis();

#endif // CHASSIS_H