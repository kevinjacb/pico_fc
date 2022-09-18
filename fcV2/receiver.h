#ifndef RECEIVER_H
#define RECEIVER_H

#include <Arduino.h>

class RECEIVER
{
private:
    const int receiver_pin[5] = {12, 13, 11, 10, 14}; // rudder,throttle,elevator,aileron

public:
    RECEIVER();
    int readPWM(int channels[]);
};

#endif