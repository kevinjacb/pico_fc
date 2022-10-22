#ifndef IMU_H
#define IMU_H

#include <Arduino.h>
#include "MPU9250.h"
#include "indicator.h"

#define ADDR 0x68

class IMU
{

private:
    MPU9250 mpu;
    MPU9250Setting settings; // mpu settings

    bool initialized = true;

    float curr_pitch, curr_roll, curr_yaw,
        prev_pitch, prev_roll, prev_yaw; // stores prev angle values

public:
    IMU(CHECKS *errorHandler);
    int updateAngles();                                        // reads from imu
    int readAngles(float *pitch, float *roll, float *yaw);     // stores pitch roll yaw into the variables
    int readPrevAngles(float *pitch, float *roll, float *yaw); // stores prev angles
};
#endif