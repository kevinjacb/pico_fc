#ifndef EEPROM_H
#define EEPROM_H

class STATE
{
private:
    // TODO
    int ESC_CALIB_ADDR = 0;

public:
    int writeTo(int mem, byte data);
    int readFrom(int mem, byte *data);
};

#endif