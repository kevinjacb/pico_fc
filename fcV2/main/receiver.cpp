#include "receiver.h"
#include <Arduino.h>
#include "indicator.h"

RECEIVER::RECEIVER(CHECKS *errorHandler)
{
    this->errorHandler = errorHandler;
    for (int i = 0; i < active_pins; i++)
        pinMode(receiver_pin[i], INPUT_PULLDOWN);
}

int RECEIVER::readPWM(int channels[])
{
    long elapsed = millis();
    for (int i = 0; i < active_pins; i++)
        channels[i] = pulseIn(receiver_pin[i], HIGH);

    if (millis() - elapsed > 3000)
        errorHandler->setError(2, 1);
    return 0;
}