# 1 "d:\\projectnightsky\\BIG DRONE\\new_fc_pico\\fcV1\\fcV1.ino"
# 2 "d:\\projectnightsky\\BIG DRONE\\new_fc_pico\\fcV1\\fcV1.ino" 2
# 3 "d:\\projectnightsky\\BIG DRONE\\new_fc_pico\\fcV1\\fcV1.ino" 2

# 5 "d:\\projectnightsky\\BIG DRONE\\new_fc_pico\\fcV1\\fcV1.ino" 2
# 6 "d:\\projectnightsky\\BIG DRONE\\new_fc_pico\\fcV1\\fcV1.ino" 2
# 7 "d:\\projectnightsky\\BIG DRONE\\new_fc_pico\\fcV1\\fcV1.ino" 2
# 8 "d:\\projectnightsky\\BIG DRONE\\new_fc_pico\\fcV1\\fcV1.ino" 2

# 10 "d:\\projectnightsky\\BIG DRONE\\new_fc_pico\\fcV1\\fcV1.ino" 2
# 22 "d:\\projectnightsky\\BIG DRONE\\new_fc_pico\\fcV1\\fcV1.ino"
const int motor_pin[] = {19, 18, 17, 16};
const int receiver_pin[] = {12, 13, 11, 10, 14}; // rudder,throttle,elevator,aileron
const int button_pin[] = {7, 8, 9, 20}; // 4th button is disconnected currently
const byte tw = 160, th = 128;

byte button_stat[] = {0, 0, 0, 0};

volatile int channel_in[4] = {1000, 1000, 1000, 1000};
volatile int channel_out[4] = {1000, 1000, 1000, 1000};

float pitch, roll, yaw,
    prev_pitch, prev_roll, prev_yaw,
    desired_pitch = 0, desired_roll = 0, desired_yaw = 0;

float kp = 1.5, kd = 0.4, ki = 0.005,
      pitch_p, pitch_d, pitch_i = 0,
      roll_p, roll_d, roll_i = 0,
      yaw_p, yaw_d, yaw_i = 0;

int pid_pitch, pid_roll, pid_yaw;
int addr = 0;

String init_screen = "Hazardouz";

MPU9250 mpu;
MPU9250Setting setting;
Servo output[4];
Adafruit_ST7735 tft = Adafruit_ST7735(4, 6, 3, 2, 5);
GUI tftHelper = GUI(&tft);
VAR variables = VAR();

void setup()
{
  // put your setup code here, to run once:
  pinMode((25u), OUTPUT);
  analogWrite((25u), 110);
  for (int i = 0; i < 4; i++)
  {
    output[i].attach(motor_pin[i]);
    output[i].writeMicroseconds(1000);
  }

  Wire.setSDA(0);
  Wire.setSCL(1);
  Wire.setClock(400000);
  Wire.begin();

  Serial.begin(115200);
  EEPROM.begin(1024);
  Serial.println("Initializing mpu");
  while (!mpu.setup(0x68, setting))
    ;

  Serial.println("IMU Calibration begins in 2 secs. Do not move");
  delay(2000);
  analogWrite((25u), 255);
  mpu.calibrateAccelGyro();
  analogWrite((25u), 110);

  Serial.println("Mag calibration begins in 2 secs. Move in 8 till done");
  delay(2000);
  analogWrite((25u), 255);
  mpu.calibrateMag();
  analogWrite((25u), 110);

  mpu.setMagneticDeclination(-0.02356194);
  Serial.println("Calibration complete");
  mpu.verbose(false);

  setting.accel_fs_sel = ACCEL_FS_SEL::A8G;
  setting.gyro_fs_sel = GYRO_FS_SEL::G250DPS;
  setting.gyro_dlpf_cfg = GYRO_DLPF_CFG::DLPF_20HZ;
  setting.accel_dlpf_cfg = ACCEL_DLPF_CFG::DLPF_21HZ;

  mpu.update();
  prev_pitch = pitch = mpu.getPitch();
  prev_roll = roll = mpu.getRoll();
  desired_yaw = yaw = prev_yaw = mpu.getYaw();
}

void setup1()
{
  String *items = variables.menu_items;
  int item_count = variables.menu_item_count;
  for (int i : receiver_pin)
    pinMode(i, INPUT);
  for (int i : button_pin)
    pinMode(i, INPUT_PULLUP);

  attachInterrupt((button_pin[0]), pin1, FALLING);
  attachInterrupt((button_pin[1]), pin2, FALLING);
  attachInterrupt((button_pin[2]), pin3, FALLING);
  attachInterrupt((button_pin[3]), pin4, FALLING);
  tftHelper.displayInfo(init_screen, 2, tw / 7, th / 3, 0x001F, 0xFC00);
  delay(1000);
  tft.fillScreen(0x0000);
  tftHelper.createMenu("MENU", items, item_count);
}

byte dlay = 10; // in millis
long prev_time;
void loop()
{
  // put your main code here, to run repeatedly:
  prev_time = millis();
  mpu.update();
  pitch = mpu.getPitch();
  roll = mpu.getRoll();
  yaw = mpu.getYaw();
  pidTuner();
  calcPid();
  setMotorSpeed();
  printOutput();
  prev_pitch = pitch;
  prev_roll = roll;
  prev_yaw = yaw;
  while (millis() - prev_time < dlay)
    ;
}

/*

 * currently uses the second core for reading pwm

 * without blocking the main loop

 */
# 146 "d:\\projectnightsky\\BIG DRONE\\new_fc_pico\\fcV1\\fcV1.ino"
// Stores the current menu item selected
int curr_menu_item = -1,
    sub_menu_item = -1;

// indicates whether a menu item has been selected
bool mode_selected = false,
     sub_selected = false;

void loop1()
{
  for (int i = 0; i < 4; i++)
    channel_in[i] = pulseIn(receiver_pin[i], HIGH, 100);
  if (mode_selected)
    switch (curr_menu_item)
    {
    case 0:
      tftHelper.updateIMU(pitch, roll, yaw);
      break;
    }
  Serial.printf("curr menu item -> %d ,sub menu item -> %d", curr_menu_item, sub_menu_item);
}

void pidTuner()
{
  if (Serial.available() > 0)
  {
    String data = Serial.readString();
    float val = data.substring(1, data.length()).toFloat();
    switch (data[0])
    {
    case 'p':
      kp = val;
      break;
    case 'd':
      kd = val;
      break;
    case 'i':
      ki = val;
      break;
    }
  }
}

void printOutput()
{
  for (int val : channel_out)
    Serial.printf(" %d ", val);
  Serial.println();
  for (int val : channel_in)
    Serial.printf(" %d ", val);
  Serial.printf("\n pitch -> %f, roll -> %f, yaw -> %f\n", pitch, roll, yaw);
  Serial.printf("\n kp -> %f, kd -> %f, ki -> %f\n", kp, kd, ki);
}

void readTXAngles()
{
  float yaw_read = map(channel_in[0], 1000, 2000, -15, 15);
  desired_yaw += yaw_read;
  if (desired_yaw > 360)
    desired_yaw = abs(360 - desired_yaw);
  desired_pitch = map(channel_in[2], 1000, 2000, -30, 30);
  desired_roll = map(channel_in[3], 1000, 2000, -30, 30);
}

void calcPid()
{
  float pitch_error = pitch - desired_pitch, dp = pitch - prev_pitch,
        roll_error = roll - desired_roll, dr = roll - prev_roll,
        yaw_error = yaw - desired_yaw, dy = yaw - prev_yaw;

  float dt = 0.01; // sampled at 100hz

  pitch_p = kp * pitch_error;
  pitch_d = kd * dp / dt;
  if (abs(pitch_error) < 15)
    pitch_i += ki * pitch_error;
  pid_pitch = pitch_p + pitch_d + pitch_i;

  roll_p = kp * roll_error;
  roll_d = kd * dr / dt;
  if (abs(roll_error) < 15)
    roll_i += ki * roll_error;
  pid_roll = roll_p + roll_d + roll_i;

  yaw_p = kp * yaw_error;
  yaw_d = kd * dy / dt;
  if (abs(yaw_error) < 15)
    yaw_i += ki * yaw_error;
  pid_yaw = yaw_p + yaw_d + yaw_i;

  channel_out[0] = channel_in[1] + pid_pitch; // + pid_roll + pid_yaw;
  channel_out[1] = channel_in[1] + pid_pitch; // - pid_roll - pid_yaw;
  channel_out[2] = channel_in[1] - pid_pitch; // - pid_roll + pid_yaw;
  channel_out[3] = channel_in[1] - pid_pitch; // + pid_roll - pid_yaw;

  for (int i = 0; i < 4; i++)
    if (channel_out[i] < 1000)
      channel_out[i] = 1000;
    else if (channel_out[i] > 2000)
      channel_out[i] = 2000;
}

void setMotorSpeed()
{
  for (int i = 0; i < 4; i++)
    if (channel_in[1] > 1200)
      output[i].writeMicroseconds(channel_out[i]);
    else
      output[i].writeMicroseconds(1000);
}

void calibrateRC()
{
  // TODO
}

// void initScreen(){
//   //TODO
// }

void updateScreen()
{
  // TODO
}
bool switch_color = false;

long prev_switch_interrupt = micros(); // variable to keep track of the last time switch was pressed
void pin1()
{
  ISR(0);
}

void pin2()
{
  ISR(1);
}

void pin3()
{
  ISR(2);
}

void pin4()
{
  ISR(3);
}
void ISR(byte pin)
{
  int prev_item = curr_menu_item,
      prev_sub_item = sub_menu_item;
  if (micros() - prev_switch_interrupt < 10000)
    return;
  bool up = false;
  if (!mode_selected)
  {
    String *items = variables.menu_items;
    int item_count = variables.menu_item_count;
    switch (pin)
    {
    case 0:
      if (curr_menu_item <= 0)
      {
        curr_menu_item = item_count - 1;
        prev_item = 0;
        up = true;
      }
      else
        curr_menu_item--;
      tftHelper.highlightItem(items, curr_menu_item, prev_item, item_count);
      break;

    case 1:
      curr_menu_item = (curr_menu_item + 1) % item_count;
      tftHelper.highlightItem(items, curr_menu_item, prev_item, item_count);
      break;

    case 2:
      mode_selected = true;
      tftHelper.selectItem(curr_menu_item);
      break;
    }
    variables.curr_menu_item = curr_menu_item;
  }
  else
  { // separated switch case because i felt that was easier to understand
    int item_count = variables.sub_menu_count[curr_menu_item];
    String items[item_count];
    for (int i = 0; i < item_count; i++)
      items[i] = variables.sub_menu_items[variables.sub_menu_count_sum[curr_menu_item] - i - 1];
    switch (pin)
    {
    case 0:
      if (sub_menu_item <= 0)
      {
        sub_menu_item = item_count - 1;
        prev_item = 0;
        up = true;
      }
      else
        sub_menu_item--;
      tftHelper.highlightItem(items, sub_menu_item, prev_item, item_count, false);
      break;

    case 1:
      sub_menu_item = (sub_menu_item + 1) % item_count;
      tftHelper.highlightItem(items, sub_menu_item, prev_item, item_count, false);
      break;

    case 2:
      sub_selected = true;
      tftHelper.selectItem(sub_menu_item);
      break;
    }
  }
  prev_switch_interrupt = micros();
}
