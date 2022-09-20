// #include <Arduino.h>
#include "pid.h"
#include "receiver.h"
#include "imu.h"
#include "output.h"

#define I2C_SDA 0
#define I2C_SCL 1

IMU *mpu;
PID *pid;
CONTROL *control;
RECEIVER *receiver;

int channels[] = {1000, 1000, 1000, 1000, 1000};
int motor_output[] = {1000, 1000, 1000, 1000};
int throttle = 1000;
int prev_led = 1000;
int state = 1;

void setup()
{
    //    Serial.begin(115200);
    initialize();
    mpu = new IMU();
    pid = new PID(mpu);
    control = new CONTROL();
    receiver = new RECEIVER();
    pinMode(LED_BUILTIN, OUTPUT);
    delay(20);
}

void loop()
{
    if (millis() - prev_led > 1000)
    {
        digitalWrite(LED_BUILTIN, (state = abs(state - 1)));
        prev_led = millis();
    }
    int start = millis();
    mpu->updateAngles();

    if (throttle > 1200)
        pid->calcPID(motor_output, throttle);
    printInfo("outputs", motor_output, 4); // remove

    float pitch, roll, yaw;
    mpu->readAngles(&pitch, &roll, &yaw);
    float arr[] = {pitch, roll, yaw};
    printInfo("Angles", arr, 3);

    control->setSpeeds(motor_output);
    while (millis() - start < 10)
        ;
}

void setup1()
{
}

void loop1()
{
    receiver->readPWM(channels);
    printInfo("channels", channels, 5); // remove
    throttle = map(channels[0], 1100, 1900, 1000, 2000);
}

void printInfo(String label, int array[], int count)
{ // For debugging
    Serial.println(label);
    for (int i = 0; i < count; i++)
        Serial.printf("%d : %d, ", i + 1, array[i]);
    Serial.println();
}
void printInfo(String label, float array[], int count)
{ // For debugging
    Serial.println(label);
    for (int i = 0; i < count; i++)
        Serial.printf("%d : %f, ", i + 1, array[i]);
    Serial.println();
}

void initialize()
{
    Serial.begin(115200);
    Wire.setSDA(I2C_SDA);
    Wire.setSCL(I2C_SCL);
    Wire.setClock(400000);
    Wire.begin();
}