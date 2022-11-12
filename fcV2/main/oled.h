#ifndef OLED_H
#define OLED_H

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include <Arduino.h>

#define WIDTH 128
#define HEIGHT 64
#define DISP_ADDR 0x3C
#define OLED_RST -1

class OLED
{
private:
    Adafruit_SSD1306 display;
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
    int ordinates[type_count][2] = {{1, 1}, {1, 10}, {1, 20}, {1, 30}, {1, 40}};
    int spacing = HEIGHT / type_count;

public:
    int mid_h = HEIGHT / 2, mid_w = WIDTH / 2;

    OLED(TwoWire *wire);
    int update();
    int setText(String data, int type); // data along with the type of message
    int setText(String data, int x, int y, int txt_size = 1);
    int clear();
};

#endif