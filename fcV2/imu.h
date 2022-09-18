#ifdef IMU
#define IMU

#include <Arduino.h>

class MPU
{

private:
    MPU9250 mpu;
    MPU9250Setting settings; // mpu settings

    float curr_pitch, curr_roll, curr_yaw,
        prev_pitch, prev_roll, prev_yaw; // stores prev angle values

public:
    MPU();
    int updateAngles();                                  // reads from imu
    int readAngles(int *pitch, int *roll, int *yaw);     // stores pitch roll yaw into the variables
    int readPrevAngles(int *pitch, int *roll, int *yaw); // stores prev angles
};
#endif