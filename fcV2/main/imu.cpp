#include "imu.h"
#include "MPU9250.h"
#include "indicator.h"
#include <Arduino.h>

IMU::IMU(CHECKS *errorHandler)
{
    Serial.println("Press any key to do mpu calibration");
    // while (!Serial.available())
    //     ;
    // Serial.println("Calibrating...");
    // Serial.println("Calibration complete");

    // mpu.setMagneticDeclination(-0.02356194);
    // mpu.verbose(false);

    settings.accel_fs_sel = ACCEL_FS_SEL::A8G;
    settings.gyro_fs_sel = GYRO_FS_SEL::G250DPS;
    settings.gyro_dlpf_cfg = GYRO_DLPF_CFG::DLPF_20HZ;
    settings.accel_dlpf_cfg = ACCEL_DLPF_CFG::DLPF_21HZ;

    long start_time = millis();
    while (!mpu.setup(ADDR, settings))
    {
        if (millis() - start_time > 10000)
        {
            initialized = false;
            break;
        }
    }

    if (!initialized)
    {
        errorHandler->setError(1);
        return;
    }
    errorHandler->setError(4); // starts calibration

    mpu.calibrateAccelGyro();
    mpu.calibrateMag();

    errorHandler->setError(0);

    errorHandler -

        updateAngles();
    readAngles(&prev_pitch, &prev_roll, &prev_yaw);
}

int IMU::updateAngles()
{ // reads current angles measured by mpu
    prev_pitch = curr_pitch;
    prev_roll = curr_roll;
    prev_yaw = curr_yaw;
    if (!mpu.update())
        return 1;
    curr_pitch = mpu.getPitch();
    curr_roll = mpu.getRoll();
    curr_yaw = mpu.getYaw();

    return 0;
}

int IMU::readAngles(float *pitch, float *roll, float *yaw)
{
    *pitch = curr_pitch;
    *roll = curr_roll;
    *yaw = curr_yaw;
    return 0;
}

int IMU::readPrevAngles(float *pitch, float *roll, float *yaw)
{ // reads last angles measured by mpu
    *pitch = prev_pitch;
    *roll = prev_roll;
    *yaw = prev_yaw;

    return 0;
}
