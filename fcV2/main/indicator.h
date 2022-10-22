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

/* MUTEX CONFIG:
        0 -> imu
        1 -> receiver
        */

class CHECKS
{
private:
    const int indi_g = 2, indi_b = 3, indi_r = 6; // green, blue, red led pins
    int error = 0,
        delay_b = 0,
        delay_g = 0,
        delay_r = 0;
    int mutex = -1;
    bool state[3] = {false, false, false}; // bgr
    long p_time[3] = {0, 0, 0};            // delay in milliseconds for blinking leds
    bool armed = false;

public:
    CHECKS();
    void setStates();
    bool setError(int c_state, int mx = -1, bool done = true); // state, accessing mutex id, hold mutex till done
    void blink(long c_time);                                   // blinks the leds
    int ok();                                                  // returns whether armable
};

#endif