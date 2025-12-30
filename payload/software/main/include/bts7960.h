/*****************************************
 * 
 * BTS7960 Motor Driver Header File
 * 
 *****************************************/
#ifndef BTS7960_H
#define BTS7960_H
/************** DEFINES **************/
#define BTS7960_RPWM_PIN        6
#define BTS7960_LPWM_PIN        7
#define MAX_SPEED               255
/************** EXTERNAL FUNCTIONS **************/
//Init
void initBTS7960();
// Forward Drill
void BTS7960Forward(int speed = 255);
// Backward Drill
void BTS7960Backward(int speed = 255);
// Stop Drill
void BTS7960Stop();
// Break Drill

// Set drill speed
void BTS7960SetSpeed(int speed);
#endif // BTS7960_H