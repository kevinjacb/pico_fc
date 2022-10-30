#ifndef EEPROM_H
#define EEPROM_H

class STATE
{
private:
    // TODO
    int ESC_CALIB_ADDR = 0,
        state_count = 1;

public:
    int writeTo(int mem, byte data);
    byte readFrom(int mem);
    int states();
};

#endif