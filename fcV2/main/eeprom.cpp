#include <Arduino.h>
#include <EEPROM.h>
#include "eeprom.h"

int STATE::writeTo(int mem, byte data)
{
    EEPROM.update(mem, data);
    return 0;
}

byte STATE::readFrom(int mem)
{
    byte data = EEPROM.read(mem);
    return data;
}

int STATE::states()
{
    return state_count;
}