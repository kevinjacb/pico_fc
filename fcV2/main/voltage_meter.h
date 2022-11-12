#ifndef VLG_H
#define VLG_H

#include <Arduino.h>

class VOLTAGE
{

private:
    const int volt_pin = A0; // ADC pin to which the voltage divider is connected
    int cell_count = 3;      // set your cell_count here!
    bool active;
    float diode_offset = 0.146; // put your diode offset value here, set it to 0 if no diode(not recommended)
    float raw_value, output_voltage;
    float calcd_voltage_divider = 6.17; // run the calibrate() to get these values
    // Used divider: 100kOhms and 15kOhms

public:
    VOLTAGE(bool active = false);
    float measure_voltage();
    int measure_voltage(float *raw, float *measured);
    int calibrate();
};

#endif