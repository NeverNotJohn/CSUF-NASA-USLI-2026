/*****************************************
 * 
 * Timer Source File
 * 
 *****************************************/

#include "timeUSLI.h"
#include "defines.h"

time_t getTeensyTime()
{
    return Teensy3Clock.get();
}

void timeSetup(int year, int month, int day, int hour, int minute, int second)
{
    tmElements_t tm;
    tm.Year = CalendarYrToTm(year);
    tm.Month = month;
    tm.Day = day;
    tm.Hour = hour;
    tm.Minute = minute;
    tm.Second = second;
    Teensy3Clock.set(makeTime(tm));
}