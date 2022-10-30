// #include <Arduino.h>
#include "pid.h"
#include "receiver.h"
#include "imu.h"
#include "output.h"
#include "indicator.h"
#include "eeprom.h"
// #include <SoftwareSerial.h>

#define I2C_SDA 4
#define I2C_SCL 5
#define BL_TX 0
#define BL_RX 1

IMU *mpu;
PID *pid;
CONTROL *control;
RECEIVER *receiver;
CHECKS *errorHandler = NULL;
STATE *mem;
// SoftwareSerial bl(BL_RX,BL_TX)

bool initialized = false;

// int channels[5] = {1000, 1000, 1000, 1000, 1000};
int motor_output[4] = {1000, 1000, 1000, 1000};
int throttle = 1000, aileron = 1000, elevator = 1000, rudder = 1000; // TODO set presets
// int prev_led = 1000;
// int state = 1;

const int throttle_thresh = 1150,
          arm_thresh = 1150;
long arm_started = 0;

void setup()
{
    //    Serial.begin(115200);

    throttle = aileron = elevator = rudder = 1000;
    initialize();
    errorHandler = new CHECKS();
    mpu = new IMU(errorHandler);
    pid = new PID(mpu);
    control = new CONTROL();
    mem = new STATE();
    // pinMode(LED_BUILTIN, OUTPUT);

    readStates();

    initialized = true;

    delay(20);
}

void loop()
{
    errorHandler->blink(millis()); // makes sure the error is displayed

    if (errorHandler->ok() == 1)
        return;
    if (receiver->getMode() == 1)
    {
        if (errorHandler->ok() != 6)
            mem->writeTo(0, 1);
        errorHandler->setError(6, 1, false);
        return;
    }
    // if (millis() - prev_led > 1000)
    // {
    //     digitalWrite(LED_BUILTIN, (state = abs(state - 1)));
    //     prev_led = millis();
    // }
    int start = millis();
    mpu->updateAngles();

    if (receiver->armSequence())
    {
        for (int i = 0; i < 4; i++)
            motor_output[i] = arm_thresh;
        control->setSpeeds(motor_output);
        errorHandler->setError(5, 2);
        if (!arm_started)
            arm_started = millis();
        // pid->calcPID(motor_output, throttle,aileron,elevator,rudder);
        // while pid testing *remove
    }
    else if (arm_started)
    {

        if (throttle < arm_thresh)
        {
            if (millis() - arm_started > 3000)
            {
                arm_started = 0;
                control->turnOff(motor_output);
            }
        }
        else
        {
            pid->calcPID(motor_output, throttle);
        }
    }
    else
        control->turnOff(motor_output);

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
    while (errorHandler == NULL)
        ;
    receiver = new RECEIVER(&errorHandler);
}

// All serial printing in the second loop
void loop1()
{

    errorHandler->blink(millis());
    receiver->readPWM(&aileron, &elevator, &throttle, &rudder);
    pidInp();
    printInfo("outputs", motor_output, 4); // remove
    receiver->display();
    // printInfo("channels", channels, 5); // remove
}

void pidInp()
{
    String data;
    if (Serial1.available() > 0)
    {
        data = Serial1.readString();
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
    Serial1.begin(115200);
    Wire.setSDA(I2C_SDA);
    Wire.setSCL(I2C_SCL);
    Wire.setClock(400000);
    Wire.begin();
}

void readStates() // INCOMPLETE TODO
{
    for (int i = 0; i < mem->states(); i++)
    {
        byte state = mem->readFrom(i);
        if (i == 0 && state)
        {
            while (errorHandler->setError(6, 2, false))
                ;
            control->ESCCalibration();
            errorHandler->setError(0, 2);
        }
        // mem->writeTo(i, 0); TO BE UNCOMMENTED
    }
}