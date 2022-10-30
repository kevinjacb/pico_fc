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

int CONTROL::turnOff(int pwm[])
{
    for (int i = 0; i < 4; i++)
        pwm[i] = 1000;
    setSpeeds(pwm);
    return 0;
}

int CONTROL::ESCCalibration()
{
    int pwm[4];
    for (int i = 0; i < 4; i++)
        pwm[i] = 2000;
    setSpeeds(pwm);

    // to be updated TODO
    Serial.println("Unplug the escs and plug them in again.\nThen press any key to continue");
    while (!Serial.available())
        ;
    Serial.println("Calibrating ....................."); // comment debug

    long start = millis();
    while (millis() - start < 2500)
        ;
    turnOff(pwm);
    return 0;
}