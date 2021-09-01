#include <Arduino.h>
#include <pumper.h>

String relay_switch()
{
    byte state;
    if (digitalRead(RELAY))
        state = 0;
    else
        state = 1;
    digitalWrite(RELAY, state);
    //Serial.printf_P(PSTR("Текущий статус %s = %s \n"), RELAY, state);
    return String(state);
}

String relay_status()
{
    byte state;
    if (digitalRead(RELAY))
        state = 1;
    else
        state = 0;
    return String(state);
}