#include "imu.h"
#include "MPU9250.h"
using namespace IMU;

IMU()
{

    // mpu.calibrateAccelGyro();
    // mpu.calibrateMag();

    // mpu.setMagneticDeclination(-0.02356194);
    // mpu.verbose(false);

    settings.accel_fs_sel = ACCEL_FS_SEL::A8G;
    settings.gyro_fs_sel = GYRO_FS_SEL::G250DPS;
    settings.gyro_dlpf_cfg = GYRO_DLPF_CFG::DLPF_20HZ;
    settings.accel_dlpf_cfg = ACCEL_DLPF_CFG::DLPF_21HZ;

    while (!mpu.setup(ADDR, setting))
        ;
    readIMU(&prev_pitch, &prev_roll, &prev_yaw);
}

int updateAngles()
{ // reads current angles measured by mpu

    if (!mpu.update())
        return 1;
    curr_pitch = mpu.getPitch();
    curr_roll = mpu.getRoll();
    curr_yaw = mpu.getYaw();

    return 0;
}

int readAngles(int *pitch, int *roll, int *yaw)
{
    *pitch = curr_pitch;
    *roll = curr_roll;
    *yaw = curr_yaw;
}

int readPrevAngles(int *pitch, int *roll, int *yaw)
{ // reads last angles measured by mpu
    *pitch = prev_pitch;
    *roll = prev_roll;
    *yaw = prev_yaw;

    return 0;
}

int updatePrevAngles(int *pitch, int *roll, int *yaw)
{
    prev_pitch = curr_pitch;
    prev_roll = curr_roll;
    prev_yaw = curr_yaw;
}
