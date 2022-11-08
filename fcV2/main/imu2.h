#ifndef IMU2_H
#define IMU2_H

#include <Arduino.h>
#include <MPU9250_WE.h>
#include "indicator.h"

#define ADDR 0x68

class IMU2
{

private:
    MPU9250_WE mpu = MPU9250_WE(ADDR); // mpu settings

    bool initialized = true;

    float curr_pitch = 0, curr_roll = 0, curr_yaw = 0,
          prev_pitch = 0, prev_roll = 0, prev_yaw = 0; // stores prev angle values
    float p_offset = 0, r_offset = 0, y_offset = 0;

public:
    IMU2(CHECKS *errorHandler);
    int updateAngles();                                        // reads from imu
    int readAngles(float *pitch, float *roll, float *yaw);     // stores pitch roll yaw into the variables
    int readPrevAngles(float *pitch, float *roll, float *yaw); // stores prev angles
    int setOffsets(float pitch, float roll, float yaw);
};
#endif