#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "oled.h"
#include "SSD1306Ascii.h"
#include "SSD1306AsciiWire.h"

OLED::OLED()
{
    int start = millis(); // for delay
    if (!display.begin(SSD1306_SWITCHCAPVCC, DISP_ADDR))
        initialized = false;
    while (millis() - start < 2000)
        ; // delay
    display.clearDisplay();
}

int OLED::update()
{
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    for (int i = 0; i < type_count; i++)
    {
        display.setCursor(ordinates[i][0], ordinates[i][1]);
        display.print(types[i] + curr_data[i]);
    }
    display.display();
    return 0;
}

int OLED::setText(String data, int type)
{
    curr_data[type] = data;
    return 0;
}

int OLED::clearLine(int cursor_height)
{
    display.fillRect(0, cursor_height, WIDTH, 8, SSD1306_BLACK);
    return 0;
}

int OLED::setText(String data, int x, int y, int txt_size)
{
    display.setCursor(x, y);
    display.setTextSize(txt_size);
    display.setTextColor(SSD1306_WHITE);
    display.print(data);
    display.display();
    return 0;
}

int OLED::clear()
{
    display.clearDisplay();
    return 0;
}

OLED2::OLED2()
{
    int start = millis(); // for delay

#if OLED_RST == 0
    display.begin(&Adafruit128x64, DISP_ADDR);
#else
    display.begin(&Adafruit128x64, DISP_ADDR, OLED_RST);
#endif

    display.setFont(Callibri11);
    display.clear();
    display.setCursor(15, 3);
    display.set2X();
    display.print("PICO FC");

    while (millis() - start < 2000)
        ; // delay
    display.set1X();
    display.setRow(10);
    display.clear();
}

int OLED2::update()
{
    display.clear();
    for (int i = 0; i < type_count; i++)
    {
        display.setCursor(ordinates[i][0], ordinates[i][1]);
        display.print(types[i] + curr_data[i]);
    }
    return 0;
}

int OLED2::setText(String data, int type)
{
    curr_data[type] = data;
    return 0;
}

// int OLED2::clearLine(int cursor_height)
// {
//     display.fillRect(0, cursor_height, WIDTH, 8, SSD1306_BLACK);
//     return 0;
// }

int OLED2::setText(String data, int x, int y, int txt_size)
{
    display.setCursor(x, y);
    display.print(data);
    return 0;
}

int OLED2::clear()
{
    display.clear();
    return 0;
}