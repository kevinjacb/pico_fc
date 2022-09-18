#include "output.h"
#include <Servo.h>
#include <Arduino.h>

CONTROL::CONTROL()
{
    for (int i = 0; i < 4; i++)
    {
        motor[i].attach(motor_pin[i]);
        motor[i].writeMicroseconds(init_pulse);
    }
}

int CONTROL::setSpeeds(int pwm[]) // sends pwm signals to escs
{

    for (int i = 0; i < 4; i++)
        motor[i].writeMicroseconds(pwm[i]);
    return 0;
}
