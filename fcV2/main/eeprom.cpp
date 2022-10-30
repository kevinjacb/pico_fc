#include <Arduino.h>
#include "eeprom.h"
#include <hardware/flash.h>

int STATE::writeTo(int offset, int data)
{
    Serial.println("SAVEEEEEEEEEEEEEE.####################################################");
    int buffer[FLASH_PAGE_SIZE / sizeof(int)];
    int ac_offset = ((int)(PICO_FLASH_SIZE_BYTES - FLASH_SECTOR_SIZE * (offset + 1)) / 4096) * 4096;
    buffer[0] = data;
    uint32_t ints = save_and_disable_interrupts();
    flash_range_program(ac_offset, (uint8_t *)buffer, FLASH_PAGE_SIZE);
    restore_interrupts(ints);
    return 0;
}

byte STATE::readFrom(int offset)
{
    Serial.println("WHAT.####################################################");
    int ac_offset = ((int)(PICO_FLASH_SIZE_BYTES - FLASH_SECTOR_SIZE * (offset + 1)) / 4096) * 4096;
    int *data = (int *)(XIP_BASE + ac_offset);
    return *data;
}

int STATE::states()
{
    return state_count;
}