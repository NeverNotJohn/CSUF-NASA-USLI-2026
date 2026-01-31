/*****************************************
 * 
 * HiLetgo BTS7960 Motor Driver Source File
 * 
 *****************************************/

#include "bts7960.h"
#include "defines.h"
/************** FUNCTIONS **************/

void BTS7960Forward(int speed)
{
    analogWrite(BTS7960_LPWM_PIN, 0);
    delayMicroseconds(100);
    analogWrite(BTS7960_RPWM_PIN, speed);
}


void BTS7960Backward(int speed)
{
    analogWrite(BTS7960_RPWM_PIN, 0);
    delayMicroseconds(100);
    analogWrite(BTS7960_LPWM_PIN, speed);
}


void BTS7960Stop()
{
    analogWrite(BTS7960_RPWM_PIN, 0);
    analogWrite(BTS7960_LPWM_PIN, 0);
}