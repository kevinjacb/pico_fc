#ifndef OLED_H
#define OLED_H

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include <Arduino.h>
#include "SSD1306Ascii.h"
#include "SSD1306AsciiWire.h"

#define WIDTH 128
#define HEIGHT 64
#define DISP_ADDR 0x3C
#define OLED_RST -1

// OLED2
#define USE_HW_I2C 0
#define MY_OLED OLED_128x64
#define FLIP180 0
#define INVERT 0
#define SDA -1
#define SCL -1

class OLED // slows the loop from 5ms to 33ms
{
private:
    Adafruit_SSD1306 display = Adafruit_SSD1306(WIDTH, HEIGHT, &Wire, OLED_RST);
    bool initialized = true;

    /* LEVELS IN DISPLAY:
        1. Error
        2. Battery
        3. PreArm check
        4. Bluetooth
        5. GPS(planned)

    */
    static const int type_count = 5;
    String types[type_count] = {"Err: ", "Batt: ", "Prearm: ", "BL: ", "GPS: "};
    String curr_data[type_count] = {"-", "-", "-", "-", "-"};
    int ordinates[type_count][2] = {{1, 1}, {1, 8}, {1, 16}, {1, 24}, {1, 32}};
    int spacing = HEIGHT / type_count;

public:
    int mid_h = HEIGHT / 2, mid_w = WIDTH / 2;

    OLED();
    int update();
    int setText(String data, int type); // data along with the type of message
    int setText(String data, int x, int y, int txt_size = 1);
    int clearLine(int cursor_height);
    int clear();
};

class OLED2
{
private:
    SSD1306AsciiWire display;
    bool initialized = true;

    /* LEVELS IN DISPLAY:
        1. Error
        2. Battery
        3. PreArm check
        4. Bluetooth
        5. GPS(planned)

    */
    static const int type_count = 4;
    String types[type_count] = {"Err: ", "Batt: ", "Prearm: ", "GPS: "};
    String curr_data[type_count] = {"-", "-", "-", "-"};
    int ordinates[type_count][2] = {{1, 0}, {1, 2}, {1, 4}, {1, 6}};
    int spacing = HEIGHT / type_count;

public:
    int mid_h = HEIGHT / 2, mid_w = WIDTH / 2;
    OLED2();
    int update();
    int setText(String data, int type); // data along with the type of message
    int setText(String data, int x, int y, int txt_size = 1);
    // int clearLine(int cursor_height);
    int clear();
};

#endif