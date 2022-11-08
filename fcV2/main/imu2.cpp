#include "imu2.h"
#include <MPU9250_WE.h>
#include "indicator.h"
#include <Arduino.h>

IMU2::IMU2(CHECKS *errorHandler)
{
    while (!(errorHandler->setError(5, 0, false)))
        ;
    long start_time = millis();
    while (!mpu.init())
    {
        if (millis() - start_time > 10000)
        {
            initialized = false;
            break;
        }
    }

    if (!initialized)
    {
        errorHandler->setError(1, 0, false);
        return;
    }
    errorHandler->setError(4, 0, false); // starts calibration
    delay(1000);
    mpu.autoOffsets();
    mpu.enableAccDLPF(true);
    mpu.setAccDLPF(MPU9250_DLPF_3);
    mpu.enableGyrDLPF();
    mpu.setGyrDLPF(MPU9250_DLPF_3);

    delay(1000);
    xyzFloat angles = mpu.getAngles();
    p_offset = angles.x;
    r_offset = angles.y;
    y_offset = angles.z;

    Serial.println("p offset --------------------------------------> " + String(p_offset));

    errorHandler->setError(0, 0);
}

int IMU2::updateAngles()
{ // reads current angles measured by mpu
    prev_pitch = curr_pitch;
    prev_roll = curr_roll;
    prev_yaw = curr_yaw;
    xyzFloat angles = mpu.getAngles();
    curr_pitch = angles.x - p_offset;
    curr_roll = angles.y - r_offset;
    curr_yaw = angles.z - y_offset;
    return 0;
}

int IMU2::readAngles(float *pitch, float *roll, float *yaw)
{
    *pitch = curr_pitch;
    *roll = curr_roll;
    *yaw = curr_yaw;
    return 0;
}

int IMU2::readPrevAngles(float *pitch, float *roll, float *yaw)
{ // reads last angles measured by mpu
    *pitch = prev_pitch;
    *roll = prev_roll;
    *yaw = prev_yaw;

    return 0;
}
