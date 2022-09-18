#include "imu.h"
#include "MPU9250.h"

IMU::IMU()
{

    Serial.println("Press any key to do mpu calibration");
    while (!Serial.available())
        ;
    mpu.calibrateAccelGyro();
    mpu.calibrateMag();

    // mpu.setMagneticDeclination(-0.02356194);
    // mpu.verbose(false);

    settings.accel_fs_sel = ACCEL_FS_SEL::A8G;
    settings.gyro_fs_sel = GYRO_FS_SEL::G250DPS;
    settings.gyro_dlpf_cfg = GYRO_DLPF_CFG::DLPF_20HZ;
    settings.accel_dlpf_cfg = ACCEL_DLPF_CFG::DLPF_21HZ;

    while (!mpu.setup(ADDR, settings))
        ;
    updateAngles();
    readAngles(&prev_pitch, &prev_roll, &prev_yaw);
}

int IMU::updateAngles()
{ // reads current angles measured by mpu

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

void IMU::updatePrevAngles()
{
    prev_pitch = curr_pitch;
    prev_roll = curr_roll;
    prev_yaw = curr_yaw;
}