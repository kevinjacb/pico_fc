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

    raw_aileron = pulseIn(receiver_pin[0], HIGH);
    raw_elevator = pulseIn(receiver_pin[1], HIGH);
    raw_throttle = pulseIn(receiver_pin[2], HIGH);
    raw_rudder = pulseIn(receiver_pin[3], HIGH);

    // Requires radio calibrated limits! TODO
    *aileron = map(raw_aileron, 1000, 2000, -max_pr, max_pr);
    *elevator = map(raw_elevator, 1000, 2000, -max_pr, max_pr);
    *throttle = map(raw_throttle, 1100, 1900, 1000, max_throttle);
    *rudder = map(raw_rudder, 1000, 2000, -max_yaw, max_yaw);

    if (millis() - elapsed > 1000)
    {
        // Serial.println("receiver trig"); // debug
        raw_aileron = raw_elevator = raw_rudder = raw_throttle = -1;
        (*errorHandler)->setError(2, 1);
    }
    else if ((*errorHandler)->ok() == 2)
    {
        (*errorHandler)->setError(0, 1);
    }
    return 0;
}

int RECEIVER::display()
{
    Serial.println("Aileron : " + String(raw_aileron));
    Serial.println("Elevator : " + String(raw_elevator));
    Serial.println("Throttle : " + String(raw_throttle));
    Serial.println("Rudder : " + String(raw_rudder));
    return 0;
}
bool RECEIVER::armSequence()
{
    if (raw_throttle < 1200 && raw_rudder < 1100 && raw_aileron > 1900)
    {
        if ((*errorHandler)->ok())
            return false;
        if (!arm_started)
            arm_started = millis();
        else if (millis() - arm_started > 2000)
            return true;
    }
    else
    {
        if (arm_started)
            (*errorHandler)->setError(0);
        arm_started = 0;
    }
    return false;
}