#include "pid.h"
#include "output.h"
// #include "imu2.h"
#include "imu.h"
#include "MPU9250.h"
// #include "MPU9250_WE.h"

#include <Arduino.h>
#define BL_TX 0
#define BL_RX 1

PID::PID(IMU *mpu)
{
    Serial1.setTX(BL_TX);
    Serial1.setRX(BL_RX);
    Serial1.begin(230400);
    this->mpu = mpu;
}

int lastSent = 0;
// calculates and adds the pids to motor values
int PID::calcPID(int pwm[], int throttle, float desired_pitch_angle, float desired_roll_angle, float desired_yaw, long dlay)
{
    // int *pwm_copy = pwm;

    // sampleRate = dlay / 1000; // can be commented
    mpu->readAngles(&pitch, &roll, &yaw);
    mpu->readPrevAngles(&prev_pitch, &prev_roll, &prev_yaw);

    pitch_error = pitch - desired_pitch_angle;
    roll_error = roll - desired_roll_angle;
    yaw_error = yaw - prev_yaw + desired_yaw;

    // debug
    // if (millis() - last > 500)
    // {
    //     Serial.println("Pitch " + String(pitch) + " prev_pitch" + String(prev_pitch) + " Roll " + String(roll) + " Prev Roll " + String(prev_roll));
    //     last = millis();
    // }
    // debug
    // if (millis() - start > sampleRate * 1000)
    // {
    //     start = millis();
    //     pitch_d = kd * (pitch - prev_pitch) / sampleRate;
    //     roll_d = kd * (roll - prev_roll) / sampleRate;
    //     mpu->readPrevAngles(&prev_pitch, &prev_roll, &prev_yaw);
    // }

    if (kp < 0.1) // while tuning, to be removed.
        ki = 0;

    pitch_p = kp * pitch_error;
    pitch_d = kd * (pitch_error - (prev_pitch - prev_desired_pitch)) / sampleRate;
    if (abs(pitch_error) < integral_angle)
        pitch_i += pitch_error * ki;
    pitch_i = constrain(pitch_i, -integral_limit, integral_limit);
    pitch_pid = pitch_p + pitch_i + pitch_d;

    roll_p = kp * roll_error;
    roll_d = kd * (roll_error - (prev_roll - prev_desired_roll)) / sampleRate;
    if (abs(roll_error) < integral_angle)
        roll_i += roll_error * ki;
    roll_i = constrain(roll_i, -integral_limit, integral_limit);
    roll_pid = roll_p + roll_i + roll_d;

    yaw_p = yaw_kp * yaw_error;
    yaw_d = yaw_kd * (yaw_error - prev_yaw_error) / sampleRate;
    // if (abs(yaw_error) < integral_angle) // TO BE UNCOMMENTED
    //     yaw_i += yaw_error * yaw_ki;
    yaw_i = constrain(yaw_i, -integral_limit, integral_limit);
    prev_yaw_error = yaw_error;
    yaw_pid = yaw_p + yaw_d + yaw_i;

    prev_desired_pitch = desired_pitch_angle;
    prev_desired_roll = desired_roll_angle;
    // YAW PID needs to be fixed!
    // TO BE REMOVED AFTER TESTING
    // yaw_pid = 0;
    // pitch_pid = 0;

    pitch_pid = constrain(pitch_pid, -pid_limit, pid_limit);
    roll_pid = constrain(roll_pid, -pid_limit, pid_limit);
    yaw_pid = constrain(yaw_pid, -pid_limit, pid_limit);

    // adjusting pwm values for balance
    pwm[3] = constrain(throttle + constrain(pitch_pid + roll_pid + yaw_pid, -pid_limit, pid_limit), 1000, 2000);
    pwm[2] = constrain(throttle + constrain(pitch_pid - roll_pid - yaw_pid, -pid_limit, pid_limit), 1000, 2000);
    pwm[1] = constrain(throttle + constrain(-pitch_pid - roll_pid + yaw_pid, -pid_limit, pid_limit), 1000, 2000);
    pwm[0] = constrain(throttle + constrain(-pitch_pid + roll_pid - yaw_pid, -pid_limit, pid_limit), 1000, 2000);

    if (millis() - lastSent > 40) // debugging
    {
        lastSent = millis();
        // Serial1.println("pitch error : " + String(pitch_error) + " pid: " + String(pitch_pid) + " p: " + String(pitch_p) + " d: " + String(pitch_d) + " i: " + String(pitch_i));
        // Serial.println("m1: " + String(pwm[0]) + " m2: " + String(pwm[1]) + " m3: " + String(pwm[2]) + " m4: " + String(pwm[3]));
        Serial1.println("pitch error : " + String(pitch_error) + " pid: " + String(pitch_pid) + " p: " + String(pitch_p) + " d: " + String(pitch_d) + " i: " + String(pitch_i));
        Serial1.println("roll error : " + String(roll_error) + " pid: " + String(roll_pid) + " p: " + String(roll_p) + " d: " + String(roll_d) + " i: " + String(roll_i));
        // Serial.println("pitch error : " + String(pitch_error) + " pid: " + String(pitch_pid) + " p: " + String(pitch_p) + " d: " + String(pitch_d) + " i: " + String(pitch_i));
        // Serial.println("roll error : " + String(roll_error) + " pid: " + String(roll_pid) + " p: " + String(roll_p) + " d: " + String(roll_d) + " i: " + String(roll_i));
        // Serial1.println("pitch error : " + String(pitch_error) + " pid: " + String(pitch_pid) + " p: " + String(pitch_p) + " d: " + String(pitch_d) + " i: " + String(pitch_i)); // debug
    }
    return 0;
}

int PID::reset()
{ // reset all variables on throttle off
    pitch = 0;
    yaw = 0;
    roll = 0;

    prev_desired_pitch = 0;
    prev_desired_roll = 0;

    prev_yaw = 0;
    prev_yaw_error = 0;
    prev_pitch = 0;
    prev_roll = 0;

    roll_i = 0;
    pitch_i = 0;
    yaw_i = 0;

    return 0;
}