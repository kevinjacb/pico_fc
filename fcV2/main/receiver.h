#ifndef RECEIVER_H
#define RECEIVER_H

#include <Arduino.h>
#include "indicator.h"

class RECEIVER
{
private:
    CHECKS **errorHandler;
    const int receiver_pin[6] = {9, 10, 11, 12, 13, 14}; // aileron,elevator,throttle,rudder
    const int active_pins = 4;
    long arm_started = 0;
    int raw_aileron = 0, raw_elevator = 0, raw_throttle = 0, raw_rudder = 0;
    int aileron = 0, elevator = 0, throttle = 0, rudder = 0;
    const int max_throttle = 1500,
              max_pr = 45,
              max_yaw = 10;

public:
    RECEIVER(CHECKS **errorHandler);
    int readPWM(int *aileron, int *elevator, int *throttle, int *rudder);
    bool armSequence();
    int display();
};

#endif