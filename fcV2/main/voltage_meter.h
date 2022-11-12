#ifndef VLG_M
#define VLG_M

class VOLTAGE
{

private:
    const int volt_pin = 26;
    int cell_count = 3;
    bool active;
    float raw_value, measured_voltage;
    float calcd_voltage_divider = 0.0;
    VOLTAGE(bool active = false);

public:
    float measure_voltage();
    int calibrate();
}

#endif