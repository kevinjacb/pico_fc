#include "MPU9250.h"
#include <Wire.h>
#include <Servo.h>

#define I2C_SDA 0
#define I2C_SCL 1
#define ADDR 0x68
#define DEF_VAL 110

const int motor_pin[] = {19,18,17,16};
const int receiver_pin[] = {12,13,11,10,14};

volatile int channel_in[4] = {1000,1000,1000,1000};
volatile int channel_out[4] = {1000,1000,1000,1000};

float pitch, roll, yaw,
prev_pitch, prev_roll, prev_yaw,
desired_pitch = 0, desired_roll = 0, desired_yaw = 0;

float kp = 0, kd = 0, ki = 0,
pitch_p, pitch_d, pitch_i = 0,
roll_p, roll_d, roll_i = 0,
yaw_p, yaw_d, yaw_i = 0;

int pid_pitch, pid_roll, pid_yaw;

MPU9250 mpu;
MPU9250Setting setting;
Servo output[4];

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_BUILTIN, OUTPUT);
  analogWrite(LED_BUILTIN,DEF_VAL);
  for(int i = 0; i < 4; i++){
    output[i].attach(motor_pin[i]);
    output[i].writeMicroseconds(1000);
  }

  Wire.setSDA(I2C_SDA);
  Wire.setSCL(I2C_SCL);
  Wire.setClock(400000);
  Wire.begin();

  Serial.begin(115200);

  Serial.println("Initializing mpu");
  while(!mpu.setup(ADDR,setting));

  Serial.println("IMU Calibration begins in 2 secs. Do not move");
  delay(2000);
  analogWrite(LED_BUILTIN,255);
  mpu.calibrateAccelGyro();
  analogWrite(LED_BUILTIN,DEF_VAL);
  
  Serial.println("Mag calibration begins in 2 secs. Move in 8 till done");
  delay(2000);
  analogWrite(LED_BUILTIN,255);
  mpu.calibrateMag();
  analogWrite(LED_BUILTIN,DEF_VAL);

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

void setup1(){
  for(int i : receiver_pin)
    pinMode(i,INPUT);
}

byte dlay = 10; // in millis
long prev_time;
void loop() {
  // put your main code here, to run repeatedly:
  prev_time = millis();
  mpu.update();
  pidTuner();
  calcPid();
//  setMotorSpeed();
  printOutput();

  while(millis() - prev_time < dlay);
}

void pidTuner(){
  if(Serial.available() > 0){
    String data = Serial.readString();
    float val = data.substring(1,data.length()).toFloat();
    switch(data[0]){
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

void printOutput(){
  for(int val : channel_out)
    Serial.printf(" %d ",val);
  Serial.printf("\n pitch -> %d, roll -> %d, yaw -> %d\n",pitch,roll,yaw);
}

void calcPid(){
  float pitch_error = pitch - desired_pitch, dp = pitch - prev_pitch,
  roll_error = roll - desired_roll, dr = roll - prev_roll,
  yaw_error = yaw - desired_yaw, dy = yaw - prev_yaw;
  
  float dt = 0.01; //sampled at 100hz
  
  pitch_p = kp*pitch_error;
  pitch_d = kd*dp/dt;
  if(abs(pitch_error) < 15)
    pitch_i += ki*pitch_error;
  pid_pitch = pitch_p + pitch_d + pitch_i;
  
  roll_p = kp*roll_error;
  roll_d = kd*dr/dt;
  if(abs(roll_error) < 15)
    roll_i += ki*roll_error;
  pid_roll = roll_p + roll_d + roll_i;
  
  yaw_p = kp*yaw_error;
  yaw_d = kd*dy/dt;
  if(abs(yaw_error) < 15)
    yaw_i += ki*yaw_error;
  pid_yaw = yaw_p + yaw_d + yaw_i;

  channel_out[0] = channel_in[2] + pid_pitch; // + pid_roll + pid_yaw;
  channel_out[1] = channel_in[2] + pid_pitch; // - pid_roll - pid_yaw;
  channel_out[2] = channel_in[2] - pid_pitch; // - pid_roll + pid_yaw;
  channel_out[3] = channel_in[2] - pid_pitch; // + pid_roll - pid_yaw;

  for(int i = 0;i < 4; i++)
    if(channel_out[i] < 1000)
      channel_out[i] = 1000;
    else if(channel_out[i] > 2000)
      channel_out[i] = 2000;
}

void setMotorSpeed(){
  for(int i = 0; i < 4; i++)
    if(channel_in[2] > 1200)
      output[i].writeMicroseconds(channel_out[i]);  
    else
      output[i].writeMicroseconds(1000);
}
