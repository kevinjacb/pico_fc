#include "receiver.h"
#include <Arduino.h>

RECEIVER::RECEIVER()
{
    for (int i = 0; i < active_pins; i++)
        pinMode(receiver_pin[i], INPUT);
}

int RECEIVER::readPWM(int channels[])
{
    for (int i = 0; i < active_pins; i++)
        channels[i] = pulseIn(receiver_pin[i], HIGH);

    return 0;
}