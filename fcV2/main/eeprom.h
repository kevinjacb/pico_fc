#ifndef EEPROM_H
#define EEPROM_H

class STATE
{
private:
    // TODO
    int ESC_CALIB_ADDR = 0,
        state_count = 1;

public:
    int writeTo(int offset, int data); // Effective offset = 4kb*offset from rear
    byte readFrom(int offset);
    int states();
};

#endif