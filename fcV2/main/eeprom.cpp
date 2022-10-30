#include <Arduino.h>
#include <EEPROM.h>
#include "eeprom.h"

int STATE::writeTo(int mem, byte data)
{
    EEPROM.update(mem, data);
    return 0;
}

int STATE::readFrom(int mem, byte *data)
{
    *data = EEPROM.read(mem);
    return 0;
}