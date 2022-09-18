#include <Arduino.h>
#include "pid.h"
#include "receiver.h"
#include "imu.h"
#include "output.h"
#include "MPU9250.h"

IMU mpu = IMU();
PID pid = PID(&mpu);
CONTROL control = CONTROL();
RECEIVER receiver = RECEIVER();

int channels[] = {1000, 1000, 1000, 1000, 1000};
int motor_output[] = {1000, 1000, 1000, 1000};
int throttle = 1000;

void setup()
{
    Serial.begin(9600);
    Wire.begin();
    delay(2000);
}

void loop()
{
    int start = millis();
    mpu.updateAngles();
    if (throttle > 1200)
        pid.calcPID(motor_output, throttle);
    printInfo(motor_output, 4); // remove
    control.setSpeeds(motor_output);
    while (millis() - start < 10)
        ;
}

void setup1()
{
}

void loop1()
{
    receiver.readPWM(channels);
    printInfo(channels, 5); // remove
    throttle = map(channels[0], 1100, 1900, 1000, 2000);
}

void printInfo(int array[], int count)
{ // For debugging
    for (int i = 0; i < count; i++)
    {
        Serial.printf("%d : ", i + 1);
        Serial.print(array[i]);
    }
    Serial.println();
}