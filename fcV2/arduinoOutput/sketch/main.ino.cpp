#include <Arduino.h>
#line 1 "d:\\projectnightsky\\BIG DRONE\\new_fc_pico\\fcV2\\main\\main.ino"
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

#line 21 "d:\\projectnightsky\\BIG DRONE\\new_fc_pico\\fcV2\\main\\main.ino"
void setup();
#line 33 "d:\\projectnightsky\\BIG DRONE\\new_fc_pico\\fcV2\\main\\main.ino"
void loop();
#line 54 "d:\\projectnightsky\\BIG DRONE\\new_fc_pico\\fcV2\\main\\main.ino"
void setup1();
#line 58 "d:\\projectnightsky\\BIG DRONE\\new_fc_pico\\fcV2\\main\\main.ino"
void loop1();
#line 65 "d:\\projectnightsky\\BIG DRONE\\new_fc_pico\\fcV2\\main\\main.ino"
void printInfo(String label, int array[], int count);
#line 72 "d:\\projectnightsky\\BIG DRONE\\new_fc_pico\\fcV2\\main\\main.ino"
void printInfo(String label, float array[], int count);
#line 80 "d:\\projectnightsky\\BIG DRONE\\new_fc_pico\\fcV2\\main\\main.ino"
void initialize();
#line 21 "d:\\projectnightsky\\BIG DRONE\\new_fc_pico\\fcV2\\main\\main.ino"
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
