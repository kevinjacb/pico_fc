#ifndef PID_H
#define PID_H

#include <Arduino.h>
// #include "imu2.h"
#include "imu.h"

class PID
{

private:
    // IMU *mpu;
    IMU *mpu;
    float sampleRate = 0.01; // testing without the time component
    const float pid_limit = 400.0,
                integral_limit = 100.0,
                integral_angle = 25;
    long start = 0;
    // long last = 0; // debug
    float pitch, roll, yaw,
        prev_pitch = 0, prev_roll = 0, prev_yaw = 0;

    float pitch_p, pitch_d = 0.0, pitch_i = 0.0,
                   roll_p, roll_d = 0.0, roll_i = 0.0,
                   yaw_p, yaw_d = 0.0, yaw_i = 0.0;

    float pitch_error, roll_error, yaw_error,
        prev_yaw_error = 0,
        prev_pitch_err = 0,
        prev_roll_err = 0,
        pitch_pid, roll_pid, yaw_pid;

public:
    float kp = 0.0, kd = 0.0, ki = 0.0,
          yaw_kp = 1, yaw_kd = 0.0, yaw_ki = 0.001;
    PID(IMU *mpu);
    int calcPID(int pwm[], int throttle = 0, float desired_pitch_angle = 0.0, float desired_roll_angle = 0.0, float desired_yaw = 0.0, long dlay = 0.005);
    int reset();
};

#endif