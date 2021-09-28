#include <Arduino.h>
#include <Relay.h>
#include <TaskSchedulerDeclarations.h>

Relay::Relay(int controlPin)
{
    _controlPin = controlPin;
    pinMode(controlPin, OUTPUT);
}

//получить статус реле
int Relay::getStatus() { return digitalRead(_controlPin); }

//закрыть реле 
void Relay::close()
{
    digitalWrite(_controlPin, 0);
}

//открыть реле
void Relay::open()
{
    digitalWrite(_controlPin, 1);
}

// запустить задачу через TaskScheduler
void Relay::startTask()
{
    //Task t1(3000, TASK_ONCE, []{relay_switch();}, &runner, true);
    Serial.println("Start t1 TASK_ONCE");

}
