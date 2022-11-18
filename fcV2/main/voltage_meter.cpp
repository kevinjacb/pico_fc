#include "voltage_meter.h"
#include <Arduino.h>

VOLTAGE::VOLTAGE(bool active)
{
    pinMode(volt_pin, INPUT);
    this->active = active;
}

int VOLTAGE::measure_voltage(float *raw, float *measured) // raw -> in 3.3v range, measured -> in 20v range
{
    if (!active)
        return 4.2 * cell_count;
    raw_value = analogRead(volt_pin);
    float m_volt = raw_value * (3.3 / 1023) - diode_offset;
    output_voltage = m_volt * calcd_voltage_divider + diode_offset;

    *raw = m_volt;
    *measured = output_voltage;

    return 0;
}

float VOLTAGE::measure_voltage() // measured -> in 20v range
{
    if (!active)
        return 4.2 * cell_count;
    raw_value = analogRead(volt_pin);
    output_voltage = (raw_value * (3.3 / 1023)) * calcd_voltage_divider + diode_offset; // not 4093??
    return output_voltage;
}

int VOLTAGE::calibrate()
{
    if (!active)
    {
        Serial.println("Voltage measurement is deactivated.");
        return 1;
    }
    Serial.println("Enter the measured voltage.");
    while (!Serial.available())
        ;
    String data = Serial.readString();
    float actual_voltage = data.toFloat();
    float raw, output;
    measure_voltage(&raw, &output);
    calcd_voltage_divider = (actual_voltage - (raw + diode_offset)) / raw;
    Serial.println("Actual voltage: " + String(actual_voltage));
    Serial.println("Measured voltage: " + String(output));
    Serial.println("Raw: " + String(raw_value));
    Serial.println("New voltage divider value: " + String(calcd_voltage_divider));
    Serial.println("Please edit this value in the code(voltage_meter.h) if the actual and measured values differs");

    return 0;
}