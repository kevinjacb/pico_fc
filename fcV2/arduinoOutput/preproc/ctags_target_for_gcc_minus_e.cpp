# 1 "d:\\projectnightsky\\BIG DRONE\\new_fc_pico\\fcV2\\main\\main.ino"
// #include <Arduino.h>
# 3 "d:\\projectnightsky\\BIG DRONE\\new_fc_pico\\fcV2\\main\\main.ino" 2
# 4 "d:\\projectnightsky\\BIG DRONE\\new_fc_pico\\fcV2\\main\\main.ino" 2

# 6 "d:\\projectnightsky\\BIG DRONE\\new_fc_pico\\fcV2\\main\\main.ino" 2




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

void setup()
{
    //    Serial.begin(115200);

    throttle = aileron = elevator = rudder = 1000;
    initialize();
    mpu = new IMU();
    pid = new PID(mpu);
    control = new CONTROL();
    pinMode((25u), OUTPUT);

    initialized = true;

    delay(20);
}

void loop()
{
    if (millis() - prev_led > 1000)
    {
        digitalWrite((25u), (state = abs(state - 1)));
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
    Wire.setSDA(4);
    Wire.setSCL(5);
    Wire.setClock(400000);
    Wire.begin();
}
