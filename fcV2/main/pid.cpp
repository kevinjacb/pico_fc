#include "pid.h"
#include "output.h"
#include "imu.h"
#include "MPU9250.h"

#include <Arduino.h>

PID::PID(IMU *mpu)
{
    this->mpu = mpu;
}

// calculates and adds the pids to motor values
int PID::calcPID(int pwm[], int throttle, float desired_pitch_angle, float desired_roll_angle, float desired_yaw_angle)
{
    // int *pwm_copy = pwm;
    float pitch, roll, yaw,
        prev_pitch, prev_roll, prev_yaw;

    float pitch_p, pitch_d, pitch_i = 0.0,
                            roll_p, roll_d, roll_i = 0.0,
                            yaw_p, yaw_d, yaw_i = 0.0;

    float pitch_error, roll_error, yaw_error,
        pitch_pid, roll_pid, yaw_pid;

    mpu->readAngles(&pitch, &roll, &yaw);
    mpu->readPrevAngles(&prev_pitch, &prev_roll, &prev_yaw);

    pitch_error = pitch - desired_pitch_angle;
    roll_error = roll - desired_roll_angle;
    yaw_error = yaw - desired_yaw_angle;

    pitch_p = kp * pitch_error;
    pitch_d = kd * (pitch - prev_pitch) / sampleRate;
    if (abs(pitch - desired_pitch_angle) > 5)
        pitch_i += pitch_error * ki;

    pitch_pid = pitch_p + pitch_d + pitch_i;

    roll_p = kp * roll_error;
    roll_d = kd * (roll - prev_roll) / sampleRate;
    if (abs(roll - desired_roll_angle) > 5)
        roll_i += roll_error * ki;

    roll_pid = roll_p + roll_d + roll_i;

    yaw_p = kp * yaw_error;
    yaw_d = kd * (yaw - prev_yaw) / sampleRate;
    if (abs(yaw - desired_yaw_angle) > 5)
        yaw_i += yaw_error * ki;

    yaw_pid = yaw_p + yaw_d + yaw_i;

    // TO BE REMOVED AFTER TESTING
    // yaw_pid = 0;
    // pitch_pid = 0;
    //

    // adjusting pwm values for balance
    pwm[3] = throttle + pitch_pid + roll_pid + yaw_pid;
    pwm[2] = throttle + pitch_pid - roll_pid - yaw_pid;
    pwm[1] = throttle - pitch_pid - roll_pid + yaw_pid;
    pwm[0] = throttle - pitch_pid + roll_pid - yaw_pid;

    return 0;
}
