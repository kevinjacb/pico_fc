#ifndef PID_H
#define PID_H

#include <Arduino.h>
#include "imu.h"

class PID
{

private:
    float kp = 1.5, kd = 0.4, ki = 0.005;
    IMU *mpu;
    float sampleRate = 0.01;

public:
    PID(IMU *mpu);
    int calcPID(int pwm[], int throttle = 0, float desired_pitch_angle = 0.0, float desired_roll_angle = 0.0, float desired_yaw_angle = 0.0);
};

#endif