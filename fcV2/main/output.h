#ifndef OUTPUT_H
#define OUTPUT_H

#include <Arduino.h>
#include <Servo.h>

class CONTROL
{
private:
    const int init_pulse = 1000;
    Servo motor[4];
    const int motor_pin[4] = {18, 19, 17, 16}; // MOTORS : 1,2,3,4 (X CONFIG)

public:
    CONTROL();
    int setSpeeds(int pwm[]);
    int turnOff(int pwm[]);
};

#endif