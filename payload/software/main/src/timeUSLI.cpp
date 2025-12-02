/*****************************************
 * 
 * Timer Source File
 * 
 *****************************************/

#include "timeUSLI.h"
#include "defines.h"
#include "arduino_freertos.h"

time_t getTeensyTime()
{
    return Teensy3Clock.get();
}

int waitInt(const char *prompt)
{
    Serial.printf("%s\n", prompt);

    // block until something arrives
    while (Serial.available() == 0);

    // read full line
    String msg = Serial.readStringUntil('\n');

    // convert
    return msg.toInt();
}

void timeSetup()
{
    Serial.println("Setup Time!");

    int year   = waitInt("Year:");
    int month  = waitInt("Month:");
    int day    = waitInt("Day:");
    int hour   = waitInt("Hour:");
    int minute = waitInt("Minute:");
    int second = waitInt("Second:");

    tmElements_t tm;
    tm.Year   = CalendarYrToTm(year);
    tm.Month  = month;
    tm.Day    = day;
    tm.Hour   = hour;
    tm.Minute = minute;
    tm.Second = second;

    Teensy3Clock.set(makeTime(tm));
}