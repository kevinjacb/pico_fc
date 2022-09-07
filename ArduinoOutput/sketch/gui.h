#ifndef GUI_CUSTOM
#define GUI_CUSTOM

#include <Arduino.h>
#include <Adafruit_ST7735.h>
#include <Adafruit_GFX.h>
#include "variables.h"

class GUI
{
private:
  Adafruit_ST7735 *tft;
  int tw = 160, th = 128,
      init_y = 35, incr_y = 16;
  VAR *variables;

public:
  GUI(Adafruit_ST7735 *tft, VAR *variables);
  void displayInfo(String data, int text_size = 1, int x = -1, int y = -1, int16_t color = ST77XX_WHITE, int16_t bg = ST77XX_BLACK, bool clr = false, bool shift = true);
  int createMenu(String title, String items[], int count, String prefix = "-> ");
  int createHorizontalWidgets(String items[], int y, int count, String prefix = "-> ");
  int IMUScreen();
  int GPSScreen();
  int ESCScreen();
  int PIDScreen();
  int ReceiverScreen();
  int TrimmingScreen();
  int updateIMU(float pitch, float roll, float yaw);
  int updateGPS();
  int updatePID();
  int updateESC();
  int updateReceiver();
  int updateTrimming();
  int highlightItem(String items[], int item, int prev_item, int item_count, bool vertical = true);
  int selectItem(int item, bool sub = false);
};
#endif
