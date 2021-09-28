#ifndef RELAY_H
#define RELAY_H

class Relay
{
private:
    int _controlPin;

public:
    Relay(int controlPin);
    int getStatus();
    void open();
    void close();
    void startTask();
};

#endif