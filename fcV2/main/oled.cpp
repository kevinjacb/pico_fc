#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "oled.h"

OLED::OLED(TwoWire *wire)
{
    display = Adafruit_SSD1306(WIDTH, HEIGHT, wire, OLED_RST);
    int start = millis(); // for delay
    if (!display.begin(SSD1306_SWITCHCAPVCC, DISP_ADDR))
        initialized = false;
    while (millis() - start < 2000)
        ; // delay
    display.clearDisplay();
}

int OLED::update()
{
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
    update();
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