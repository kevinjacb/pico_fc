#ifndef INDIC
#define INDIC

#include <Arduino.h>
/*
    ERRORS :
        0 -> no error (green);
        1 -> failed to initialize imu (red blink 1 sec)
        2 -> no connection to receiver (blue(steady) and green (steady))
        3 -> needs calibration (blue green)(blink)

    STATE :
        4 -> calibrating (fast blinking blue)
*/
/*
    ARM : if error then no arming else arm

*/

class CHECKS
{
private:
    const byte indi_g = 2, indi_b = 3, indi_r = 6; // green, blue, red led pins
    int error = 0,
        delay_b = 0,
        delay_g = 0,
        delay_r = 0;
    bool state[3] = {false, false, false}; // bgr
    long p_time[3] = {0, 0, 0};            // delay in milliseconds for blinking leds
    bool armed = false;

public:
    CHECKS();
    void setStates();
    void setError(int c_state);
    void blink(long c_time); // blinks the leds
    bool arm();              // returns whether armable
};

#endif