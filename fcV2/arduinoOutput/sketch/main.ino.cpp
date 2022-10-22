#include <Arduino.h>
#line 1 "d:\\projectnightsky\\BIG DRONE\\new_fc_pico\\fcV2\\main\\main.ino"
// #include <Arduino.h>
#include "pid.h"
#include "receiver.h"
#include "imu.h"
#include "output.h"

#define I2C_SDA 4
#define I2C_SCL 5

IMU *mpu;
PID *pid;
CONTROL *control;
RECEIVER *receiver;

bool initialized = false;
const int throttle_thresh = 1150;

int channels[5] = {1000, 1000, 1000, 1000, 1000};
int motor_output[4] = {1000, 1000, 1000, 1000};
int throttle, aileron, elevator, rudder;
int prev_led = 1000;
int state = 1;

#line 24 "d:\\projectnightsky\\BIG DRONE\\new_fc_pico\\fcV2\\main\\main.ino"
void setup();
#line 40 "d:\\projectnightsky\\BIG DRONE\\new_fc_pico\\fcV2\\main\\main.ino"
void loop();
#line 67 "d:\\projectnightsky\\BIG DRONE\\new_fc_pico\\fcV2\\main\\main.ino"
void setup1();
#line 73 "d:\\projectnightsky\\BIG DRONE\\new_fc_pico\\fcV2\\main\\main.ino"
void loop1();
#line 89 "d:\\projectnightsky\\BIG DRONE\\new_fc_pico\\fcV2\\main\\main.ino"
void pidInp();
#line 113 "d:\\projectnightsky\\BIG DRONE\\new_fc_pico\\fcV2\\main\\main.ino"
void printInfo(String label, int array[], int count);
#line 120 "d:\\projectnightsky\\BIG DRONE\\new_fc_pico\\fcV2\\main\\main.ino"
void printInfo(String label, float array[], int count);
#line 128 "d:\\projectnightsky\\BIG DRONE\\new_fc_pico\\fcV2\\main\\main.ino"
void initialize();
#line 24 "d:\\projectnightsky\\BIG DRONE\\new_fc_pico\\fcV2\\main\\main.ino"
void setup()
{
    //    Serial.begin(115200);

    throttle = aileron = elevator = rudder = 1000;
    initialize();
    mpu = new IMU();
    pid = new PID(mpu);
    control = new CONTROL();
    pinMode(LED_BUILTIN, OUTPUT);

    initialized = true;

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
        // pid->calcPID(motor_output, throttle,aileron,elevator,rudder);
        pid->calcPID(motor_output, throttle); // while pid testing *remove
    else
        for (int i = 0; i < 4; i++)
            motor_output[i] = 1000;

    float pitch, roll, yaw;
    mpu->readAngles(&pitch, &roll, &yaw);
    float arr[] = {pitch, roll, yaw};
    // printInfo("Angles", arr, 3);

    control->setSpeeds(motor_output);
    while (millis() - start < 10)
        ;
}

void setup1()
{
    receiver = new RECEIVER();
}

// All serial printing in the second loop
void loop1()
{

    receiver->readPWM(channels);
    pidInp();
    printInfo("outputs", motor_output, 4); // remove
    // printInfo("channels", channels, 5); // remove
    throttle = map(channels[1], 1100, 1900, 1000, 2000);
    if (throttle > throttle_thresh) // set threshold
    {
        aileron = map(channels[3], 1100, 2000, -45, 45);
        elevator = map(channels[2], 1100, 2000, -45, 45);
        rudder += map(channels[0], 1100, 2000, -10, 10);
    }
}

void pidInp()
{
    String data;
    if (Serial.available() > 0)
    {
        data = Serial.readString();
        float val = (data.substring(1)).toFloat();
        switch (data[0])
        {
        case 'p':
            pid->kp = val;
            break;
        case 'i':
            pid->ki = val;
            break;
        case 'd':
            pid->kd = val;
            break;
        }
        Serial.printf("val -> %f\n", val);
        Serial.println(data);
    }
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
