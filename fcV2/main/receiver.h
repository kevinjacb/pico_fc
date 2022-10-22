#ifndef RECEIVER_H
#define RECEIVER_H

#include <Arduino.h>
#include "indicator.h"

class RECEIVER
{
private:
    CHECKS *errorHandler;
    const int receiver_pin[6] = {9, 10, 11, 12, 13, 14}; // rudder,throttle,elevator,aileron
    const int active_pins = 4;

public:
    RECEIVER(CHECKS *errorHandler);
    int readPWM(int channels[]);
};

#endif