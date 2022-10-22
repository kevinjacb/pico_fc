#include "indicator.h"
#include <Arduino.h>

CHECKS::CHECKS()
{
    pinMode(indi_b, OUTPUT);
    pinMode(indi_g, OUTPUT);
    pinMode(indi_r, OUTPUT);
}

void CHECKS::setStates()
{
    digitalWrite(indi_b, state[0]);
    digitalWrite(indi_g, state[1]);
    digitalWrite(indi_r, state[2]);
}

bool CHECKS::setError(int c_state, int mx, bool done)
{
    if (mutex != -1 && mx != mutex)
        return false;
    if (mutex == -1 && mx != -1)
        mutex = mx;
    delay_b = delay_g = delay_r = 0;
    state[0] = state[1] = state[2] = false;
    switch (c_state)
    {
    case 0:
        state[1] = true;
        setStates();
        break;
    case 1:
        state[2] = true;
        delay_r = 500;
        setStates();
        break;
    case 2:
        state[0] = state[1] = true;
        setStates();
        break;
    case 3:
        state[0] = state[1] = true;
        setStates();
        delay_b = 1000;
        break;

    case 4:
        state[0] = state[1] = true;
        setStates();
        delay_b = 200;
        break;
    }
    if (done)
        mutex = -1;
    return true;
}

void CHECKS::blink(long c_time)
{
    if (c_time - p_time[0] > delay_b && delay_b)
    {
        state[0] = !state[0];
        p_time[0] = c_time;
    }
    if (c_time - p_time[1] > delay_g && delay_g)
    {
        state[1] = !state[1];
        p_time[1] = c_time;
    }
    if (c_time - p_time[2] > delay_r && delay_r)
    {
        state[2] = !state[2];
        p_time[2] = c_time;
    }
}

int CHECKS::ok()
{
    return error;
}