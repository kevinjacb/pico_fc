#include "receiver.h"
#include <Arduino.h>

RECEIVER::RECEIVER()
{
    for (int i = 0; i < 5; i++)
        pinMode(receiver_pin[i], INPUT);
}

int RECEIVER::readPWM(int channels[])
{
    for (int i = 0; i < 5; i++)
        channels[i] = pulseIn(receiver_pin[i], HIGH, 3000);

    return 0;
}