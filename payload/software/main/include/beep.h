/*****************************************
 * 
 * Beep Header File
 * 
 * Buzzer
 *****************************************/

#ifndef BEEP_H
#define BEEP_H

/************** EXTERNAL FUNCTIONS **************/
void initBeep();
void beep(int numBeeps, double onInterval_ms, double offInterval_ms);

#endif // BEEP_H
