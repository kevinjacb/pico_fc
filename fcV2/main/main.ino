// #include <Arduino.h>
#include "pid.h"
#include "receiver.h"
// #include "imu2.h"
#include "imu.h"
#include "output.h"
#include "indicator.h"
#include "eeprom.h"
#include "voltage_meter.h"
#include "oled.h"
// #include <SoftwareSerial.h>

#define I2C_SDA 4
#define I2C_SCL 5

IMU *mpu;
// IMU2 *mpu;
PID *pid;
CONTROL *control;
RECEIVER *receiver;
CHECKS *errorHandler;
STATE *mem;
VOLTAGE *vmonitor;
OLED *screen;
// SoftwareSerial bl(BL_RX,BL_TX)

bool initialized = false,
     halt = false;

// int channels[5] = {1000, 1000, 1000, 1000, 1000};
int motor_output[4] = {1000, 1000, 1000, 1000};
int throttle = 1000, aileron = 1500, elevator = 1500, rudder = 1500; // TODO set presets
// int prev_led = 1000;
// int state = 1;

int last_err = -1;
const int throttle_thresh = 1100,
          arm_thresh = 1100;
long arm_started = 0;

void setup()
{
    //    Serial.begin(115200);

    throttle = aileron = elevator = rudder = 1000;
    initialize();
    errorHandler = new CHECKS();
    // mpu = new IMU2(errorHandler);
    mpu = new IMU(errorHandler);
    pid = new PID(mpu);
    control = new CONTROL();
    mem = new STATE();
    vmonitor = new VOLTAGE(true);
    screen = new OLED(&Wire);

    screen->setText("Enabled", 4); // TODO hardcoded atm
    screen->setText("Unavailable", 5);

    // pinMode(LED_BUILTIN, OUTPUT);

    // readStates();        //to be updated TODO

    initialized = true;

    delay(20);
}

void loop()
{
    int start = millis();
    errorHandler->blink(millis()); // makes sure the error is displayed

    if (Serial.available())
    {
        String data = Serial.readString();
        // Serial.println(data); // debug
        if (!data.equalsIgnoreCase("calibrate"))
        {
            while (!errorHandler->setError(7, 3, false))
                ;
            halt = true;
            vmonitor->calibrate();
            errorHandler->setError(0, 3);
            halt = false;
        }
    }
    if (last_err != errorHandler->ok())
    {
        last_err = errorHandler->ok();
        switch (last_err)
        {
        case -1:
            screen->setText("Failed", 3);
        case 0:
            screen->setText("None", 0);
            screen->setText("Cleared", 3);
            break;
        case 1:
            screen->setText("IMU ERR!", 0);
            break;
        case 2:
            screen->setText("no RX sig", 0);
            break;
        case 3:
            screen->setText("Calibration required", 0);
            break;
        }
    }
    if (errorHandler->ok() == 1 || errorHandler->ok() == 6) // TODO
        return;
    if (receiver->getMode() == 1)
    {
        errorHandler->setError(6, 1, false);
        return;
    }

    // Calibration check:

    mpu->updateAngles();

    if (receiver->armSequence())
    {
        for (int i = 0; i < 4; i++)
            motor_output[i] = arm_thresh;
        control->setSpeeds(motor_output);
        errorHandler->setError(5, 2);
        if (!arm_started)
            arm_started = millis();
        screen->clear();
        screen->setText(">>>> ARMED <<<<", screen->mid_h, 10);
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
                screen->clear();
            }
        }
        else
        {
            pid->calcPID(motor_output, throttle, -elevator, aileron, -rudder);
        }
    }
    else
    {
        pid->reset();
        control->turnOff(motor_output);
    }
    // float pitch, roll, yaw;
    // mpu->readAngles(&pitch, &roll, &yaw);

    // float arr[] = {pitch, roll, yaw};
    // printInfo("Angles", arr, 3);

    control->setSpeeds(motor_output);
    while (millis() - start < 5)
        ;

    // Serial.println(millis() - start); // debug
}

void setup1()
{
    while (!initialized)
        ;
    receiver = new RECEIVER(&errorHandler);
}

// All serial printing in the second loop
void loop1()
{

    if (halt)
    {
        control->turnOff(motor_output);
        return;
    }
    if (errorHandler->ok() == 6)
    {
        control->turnOff(motor_output);
        control->ESCCalibration();
        errorHandler->setError(0, 1);
    }
    errorHandler->blink(millis());
    receiver->readPWM(&aileron, &elevator, &throttle, &rudder);
    pidInp();
    float p, d;
    receiver->getPIDValues(&p, &d);
    pid->kp = p;
    pid->kd = d;
    // Serial.println(" PID p : " + String(p) + " PID d : " + String(d));
    // printInfo("outputs", motor_output, 4); // remove
    // receiver->display();
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
        case 'a':
            pid->yaw_kp = val;
            break;
        case 'b':
            pid->yaw_ki = val;
            break;
        case 'c':
            pid->yaw_kd = val;
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

// to be updated        TODO
//  void readStates() // INCOMPLETE TODO
//  {
//      for (int i = 0; i < mem->states(); i++)
//      {
//          // byte state = mem->readFrom(i);
//          if (i == 0 && state)
//          {
//              Serial.println("time for calibration!");
//              while (!errorHandler->setError(6, 2, false))
//                  ;
//              control->ESCCalibration();
//              errorHandler->setError(0, 2);
//              // mem->writeTo(i, 0); // TO BE UNCOMMENTED
//          }
//      }
//  }