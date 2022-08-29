#ifndef GUI_CUSTOM
#define GUI_CUSTOM

#include <Arduino.h>
#include <Adafruit_ST7735.h>
#include <Adafruit_GFX.h>

class GUI{
  private:
    Adafruit_ST7735 *tft;
    int tw = 160,th = 128,
    init_y = 35,incr_y = 16;
  public:
    GUI(Adafruit_ST7735 *tft);
    void displayInfo(String data, int text_size = 1, int x = -1, int y = -1, int16_t color = ST77XX_WHITE, int16_t bg = ST77XX_BLACK, bool clr=false, bool shift = true);
    int createMenu(String title, String items[],int count);
    int IMUScreen();
    int GPSScreen();
    int ESCScreen();
    int PIDScreen();
    int RecieverScreen();
    int TrimmingScreen();
    int highlightItem(String items[],int item,int prev_item,int item_count);
    int selectItem(int item);
};
#endif
