#include <Arduino.h>
#include "pid.h"
#include "imu.h"
#include "output.h"
#include "MPU9250.h"

IMU mpu = IMU();
PID pid = PID(&mpu);
CONTROL control = CONTROL();

void setup()
{
    Serial.begin(9600);
    Wire.begin();
    delay(2000);
}

void loop()
{
}