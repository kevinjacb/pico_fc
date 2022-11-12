#ifndef VLG_H
#define VLG_H

#include <Arduino.h>

class VOLTAGE
{

private:
    const int volt_pin = 26;
    int cell_count = 3;
    bool active;
    float diode_offset = 0.146; // put your diode offset value here, set it to 0 if no diode(not recommended)
    float raw_value, output_voltage;
    float calcd_voltage_divider = 0.0; // run the calibrate() to get these values
    VOLTAGE(bool active = false);

public:
    float measure_voltage();
    int measure_voltage(float *raw, float *measured);
    int calibrate();
};

#endif