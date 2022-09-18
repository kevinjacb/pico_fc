#ifndef OUTPUT_H
#define OUTPUT_H

#include <Arduino.h>
#include <Servo.h>

class CONTROL
{
private:
    const int init_pulse = 1000;
    Servo motor[4];
    const int motor_pin[4] = {19, 18, 17, 16};

public:
    CONTROL();
    int setSpeeds(int pwm[]);
};

#endif