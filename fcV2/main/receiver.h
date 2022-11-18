#ifndef RECEIVER_H
#define RECEIVER_H

#include <Arduino.h>
#include "indicator.h"

class RECEIVER
{
private:
    CHECKS **errorHandler;
    const int receiver_pin[6] = {9, 10, 11, 12, 13, 14}; // aileron,elevator,throttle,rudder
    const int active_pins = 6;
    long arm_started = 0;
    int raw_aileron = 0, raw_elevator = 0, raw_throttle = 0, raw_rudder = 0, raw_mode = 0, raw_dval = 0;
    int aileron = 0, elevator = 0, throttle = 0, rudder = 0, mode = 0;
    float pid_pval = 0, pid_dval = 0;
    const int max_throttle = 1700, // to be changed after tuning
        max_pr = 25,
              max_yaw = 25;

public:
    RECEIVER(CHECKS **errorHandler);
    int readPWM(int *aileron, int *elevator, int *throttle, int *rudder);
    bool armSequence();
    int display();
    int getMode();                        // testing
    int getPIDValues(float *p, float *d); // testing
};

#endif