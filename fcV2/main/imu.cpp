#include "imu.h"
#include "MPU9250.h"
#include "indicator.h"
#include <Arduino.h>

IMU::IMU(CHECKS *errorHandler)
{
    // Serial.println("Press any key to do mpu calibration");
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

    while (!(errorHandler->setError(5, 0, false)))
        ;
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
        errorHandler->setError(1, 0, false);
        return;
    }
    errorHandler->setError(4, 0, false); // starts calibration
    mpu.verbose(true);

    // Serial.println("Calibrating the gyro");
    // mpu.calibrateAccelGyro();
    // Serial.println("Calibrating mag in 5 secs");
    // delay(5000);
    // mpu.calibrateMag();
    // mpu.selectFilter(QuatFilterSel::MAHONY);
    // mpu.setFilterIterations(10);

    // FIX CALIBRATION
    // float accBiasX = mpu.getAccBiasX() * 1000.f / (float)MPU9250::CALIB_ACCEL_SENSITIVITY;
    // float accBiasY = mpu.getAccBiasY() * 1000.f / (float)MPU9250::CALIB_ACCEL_SENSITIVITY;
    // float accBiasZ = mpu.getAccBiasZ() * 1000.f / (float)MPU9250::CALIB_ACCEL_SENSITIVITY;

    // float gyroBiasX = mpu.getGyroBiasX() / (float)MPU9250::CALIB_GYRO_SENSITIVITY;
    // float gyroBiasY = mpu.getGyroBiasY() / (float)MPU9250::CALIB_GYRO_SENSITIVITY;
    // float gyroBiasZ = mpu.getGyroBiasZ() / (float)MPU9250::CALIB_GYRO_SENSITIVITY;

    // mpu.setAccBias(accBiasX, accBiasY, accBiasZ);
    // mpu.setGyroBias(gyroBiasX, gyroBiasY, gyroBiasZ);

    // old values :
    //  mpu.setAccBias(849.76, -561.07, 1025.07);
    //  mpu.setGyroBias(39.98, 294.41, 113.59);
    //  mpu.setMagBias(-122.43, 110.35, 81.37);

    mpu.setAccBias(753.27, -446.93, 1055.02);
    mpu.setGyroBias(41.95, 303.32, 113.12);
    mpu.setMagBias(184.57, 336.55, 24.76);

    mpu.verbose(false);

    errorHandler->setError(0, 0);
}

int IMU::updateAngles()
{ // reads current angles measured by mpu
    prev_pitch = curr_pitch;
    prev_roll = curr_roll;
    prev_yaw = curr_yaw;
    if (!mpu.update())
        return 1;
    curr_pitch = mpu.getPitch() - p_offset;
    curr_roll = mpu.getRoll() - r_offset;
    curr_yaw = mpu.getYaw() - y_offset;
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

// not used
int IMU::setOffsets()
{
    for (int i = 0; i < 1000; i++)
    {
        float p, r, y;
        updateAngles();
        readAngles(&p, &r, &y);
        p_offset += p;
        r_offset += r;
        y_offset += y;
        delay(5);
    }
    p_offset /= 1000;
    r_offset /= 1000;
    y_offset /= 1000;
    return 0;
}
