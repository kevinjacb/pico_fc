#include "pid.h"
#include "output.h"
#include "imu.h"
#include "MPU9250.h"

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
int PID::calcPID(int pwm[], int throttle, float desired_pitch_angle, float desired_roll_angle, float desired_yaw)
{
    // int *pwm_copy = pwm;

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

    pitch_p = kp * pitch_error;
    pitch_d = kd * (pitch - prev_pitch) / sampleRate;
    if (abs(pitch_error) < 5)
        pitch_i += pitch_error * ki;

    pitch_pid = pitch_p + pitch_i + pitch_d;

    roll_p = kp * roll_error;
    roll_d = kd * (roll - prev_roll) / sampleRate;
    if (abs(roll_error) < 5)
        roll_i += roll_error * ki;

    roll_pid = roll_p + roll_i + roll_d;

    yaw_p = yaw_kp * yaw_error;
    yaw_d = yaw_kd * (yaw_error - prev_yaw_error) / sampleRate;
    if (abs(roll_error) < 5)
        yaw_i += yaw_error * yaw_ki;
    prev_yaw_error = yaw_error;
    yaw_pid = yaw_p + yaw_d + yaw_i;

    // YAW PID needs to be fixed!
    // TO BE REMOVED AFTER TESTING
    // yaw_pid = 0;
    // pitch_pid = 0;

    if (millis() - lastSent > 40)
    {
        lastSent = millis();
        Serial1.println("pitch error : " + String(pitch_error) + " pid: " + String(pitch_pid) + " p: " + String(pitch_p) + " d: " + String(pitch_d) + " i: " + String(pitch_i));
        Serial.println("pitch error : " + String(pitch_error) + " pid: " + String(pitch_pid) + " p: " + String(pitch_p) + " d: " + String(pitch_d) + " i: " + String(pitch_i));
        Serial.println("roll error : " + String(roll_error) + " pid: " + String(roll_pid) + " p: " + String(roll_p) + " d: " + String(roll_d) + " i: " + String(roll_i));
        // Serial1.println("pitch error : " + String(pitch_error) + " pid: " + String(pitch_pid) + " p: " + String(pitch_p) + " d: " + String(pitch_d) + " i: " + String(pitch_i)); // debug
    }
    pitch_pid = constrain(pitch_pid, -pid_limit, pid_limit);
    roll_pid = constrain(roll_pid, -pid_limit, pid_limit);
    yaw_pid = constrain(yaw_pid, -pid_limit, pid_limit);

    // adjusting pwm values for balance
    pwm[3] = constrain(throttle + constrain(pitch_pid + roll_pid + yaw_pid, -pid_limit, pid_limit), 1000, 2000);
    pwm[2] = constrain(throttle + constrain(pitch_pid - roll_pid - yaw_pid, -pid_limit, pid_limit), 1000, 2000);
    pwm[1] = constrain(throttle + constrain(-pitch_pid - roll_pid + yaw_pid, -pid_limit, pid_limit), 1000, 2000);
    pwm[0] = constrain(throttle + constrain(-pitch_pid + roll_pid - yaw_pid, -pid_limit, pid_limit), 1000, 2000);

    return 0;
}
