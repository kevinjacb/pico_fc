#include "receiver.h"
#include <Arduino.h>
#include "indicator.h"

RECEIVER::RECEIVER(CHECKS **errorHandler)
{
    this->errorHandler = errorHandler;
    for (int i = 0; i < active_pins; i++)
        pinMode(receiver_pin[i], INPUT_PULLDOWN);
}

int RECEIVER::readPWM(int *aileron, int *elevator, int *throttle, int *rudder)
{
    long elapsed = millis();

    // Requires radio calibrated limits! TODO
    *aileron = map(pulseIn(receiver_pin[0], HIGH), 1000, 2000, -max_pr, max_pr);
    *elevator = map(pulseIn(receiver_pin[1], HIGH), 1000, 2000, -max_pr, max_pr);
    *throttle = map(pulseIn(receiver_pin[2], HIGH), 1100, 1900, 1000, max_throttle);
    *rudder = map(pulseIn(receiver_pin[3], HIGH), 1000, 2000, -max_yaw, max_yaw);

    if (millis() - elapsed > 1000)
    {
        // Serial.println("receiver trig"); // debug
        (*errorHandler)->setError(2, 1);
    }
    return 0;
}

bool RECEIVER::armSequence()
{
    if (throttle < 1200 && rudder < 1100 && elevator > 1900)
    {
        if (!arm_started)
            arm_started = millis();
        else if (millis() - arm_started > 2000)
            return true;
    }
    else
    {
        arm_started = 0;
    }
    return false;
}