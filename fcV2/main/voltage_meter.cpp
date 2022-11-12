#include "voltage_meter.h"
#include "Arduino.h"

VOLTAGE::VOLTAGE(bool active)
{
    pinMode(volt_pin, INPUT);
    this->active = active;
}

float VOLTAGE::measure_voltage()
{
    if (!active)
        return 4.2 * cell_count;
    raw_value = analogRead(volt_pin);
    measured_voltage = raw_value * (3.3 / 4093) * calcd_voltage_divider;
    return measure_voltage;
}

int VOLTAGE::calibrate()
{
    return 0;
}