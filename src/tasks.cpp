#include <Arduino.h>
#include <pumper.h>

#define NUMBER_OF_TASKS 1;

//TickerScheduler taskSch;// = NUMBER_OF_TASKS;

void relay_start(int time, int delay)
{
    taskSch.add(
        0, time*1000, [&](void *)
        { relay_switch(); },
        nullptr, true);
}
