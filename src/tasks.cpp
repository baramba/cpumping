#include <Arduino.h>
#include <pumper.h>

#include <TaskSchedulerDeclarations.h>

extern Scheduler runner;

void relay_start(int time, int delay)
{
   Task t1(3000, TASK_ONCE, []{relay_switch();}, &runner, true);
    Serial.println("Start t1 TASK_ONCE");
    // taskSch.add(
    //     0, time*1000, [&](void *)
    //     { relay_switch(); },
    //     nullptr, true);

}
