#include <Arduino.h>
#include <pumper.h>

class relay
{
public:
    int controlPin;

    int getStatus()
    {
        return digitalRead(controlPin);
    }
    void open(){
        digitalWrite(controlPin, 1);
    }
    void close(){
        digitalWrite(controlPin, 0);
    }

private:
};
