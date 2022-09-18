#include "gui.h"
#include <Adafruit_ST7735.h>
#include <Adafruit_GFX.h>
#include <iostream>
using namespace std;

GUI::GUI(Adafruit_ST7735 *tft, VAR *variables) : tft(tft)
{
  tft->initR(INITR_BLACKTAB);
  tft->fillScreen(ST77XX_BLACK);
  tft->setRotation(1);
  this->variables = variables;
}

void GUI::displayInfo(String data, int text_size, int x, int y, int16_t color, int16_t bg, bool clr, bool shift)
{
  if (x != -1 && y != -1)
    tft->setCursor(x, y);
  if (clr)
    tft->fillScreen(ST77XX_BLACK);
  tft->setTextColor(color, bg);
  tft->setTextSize(text_size);
  if (shift)
    tft->println(data);
  else
    tft->print(data);
}

int GUI::createMenu(String title, String items[], int count, String prefix)
{
  tft->drawRect(0, 0, tw, th, ST77XX_ORANGE);
  GUI::displayInfo(title, 2, tw / 3, 8, ST77XX_YELLOW);
  tft->drawLine(0, init_y - 8, tw, init_y - 8, ST77XX_YELLOW);
  for (int i = 0, y_offset = 0; i < count; i++, y_offset += incr_y)
    GUI::displayInfo(prefix + items[i], 1, tw / 10, init_y + y_offset, ST77XX_CYAN);
  return 1;
}

int GUI::createHorizontalWidgets(String items[], int y, int count, String prefix)
{
  int w_space = tw / count, w_offset = 10;
  for (int i = 0; i < count; i++)
    GUI::displayInfo(prefix + items[i], 1, w_offset + (w_space + 10) * i, y, ST77XX_CYAN, ST77XX_BLACK);
  return 1;
}

int GUI::IMUScreen()
{
  String items[] = {"PITCH: ", "ROLL: ", "YAW: "};
  int item_count = 3, option_count, h = init_y + incr_y * (item_count + 1);
  String *options;
  variables->getMenu(&options, &option_count);
  Serial.printf("String : %s\n", options[0]);
  tft->drawRect(0, 0, tw, th, ST77XX_RED);
  GUI::createMenu("IMU", items, item_count, "~");
  GUI::createHorizontalWidgets(options, h, option_count);
  tft->drawRect(0, h - 7, tw, 23, ST77XX_YELLOW);
  tft->drawLine(tw / 2 + 10, h - 7, tw / 2 + 10, h + 15, ST77XX_YELLOW);
  tft->drawRect(0, 0, tw, th, ST77XX_GREEN);
  return 1;
}

int GUI::updateIMU(float pitch, float roll, float yaw)
{
  GUI::displayInfo(String(pitch), 1, tw / 2, init_y, ST77XX_WHITE);
  GUI::displayInfo(String(roll), 1, tw / 2, init_y + incr_y, ST77XX_WHITE);
  GUI::displayInfo(String(yaw), 1, tw / 2, init_y + incr_y * 2, ST77XX_WHITE);
  return 1;
}

int GUI::GPSScreen()
{
  // TODO
  return 1;
}

int GUI::updateGPS()
{
  // TODO
  return 1;
}

int GUI::ESCScreen()
{
  // TODO
  return 1;
}

int GUI::updateESC()
{
  // TODO
  return 1;
}

int GUI::PIDScreen()
{
  // TODO
  return 1;
}

int GUI::updatePID()
{
  // TODO
  return 1;
}

int GUI::ReceiverScreen()
{
  // TODO
  return 1;
}

int GUI::updateReceiver()
{
  // TODO
  return 1;
}

int GUI::TrimmingScreen()
{
  // TODO
  return 1;
}

int GUI::updateTrimming()
{
  // TODO
  return 1;
}

int GUI::highlightItem(String *items, int item, int prev_item, int item_count, bool vertical)
{
  if (vertical) // scroll vertically
  {
    if (prev_item != -1)
      GUI::displayInfo("-> " + items[prev_item], 1, tw / 10, init_y + incr_y * prev_item, ST77XX_CYAN, ST77XX_BLACK, false, false);
    GUI::displayInfo("-> " + items[item], 1, tw / 10, init_y + incr_y * item, ST77XX_BLACK, ST77XX_WHITE, false, false);
  }
  else // scroll horizontally
  {
    int sub_list_count = variables->menu_content_list_count[variables->curr_menu_item];
    int w_space = tw / item_count, w_offset = 10;
    String pitem = String("-> " + items[prev_item]), citem = String("-> " + items[item]);
    // if (prev_item != -1)
    //   GUI::displayInfo(pitem, 1, w_offset + (w_space + 10) * prev_item, init_y + incr_y * (sub_list_count + 1), ST77XX_CYAN, ST77XX_BLACK, false, false);
    // GUI::displayInfo(citem, 1, w_offset + (w_space + 10) * item, init_y + incr_y * (sub_list_count + 1), ST77XX_BLACK, ST77XX_WHITE, false, false);

    Serial.printf("citem = %d, pitem = %d, item_count = %d, sub_list = %d", item, prev_item, item_count, sub_list_count);
    return 1;
  }
}

int GUI::selectItem(int item, bool sub)
{
  switch (item)
  {
  case 0:
    tft->fillScreen(ST77XX_BLACK);
    GUI::IMUScreen();
    break;
  }
  return 1;
}
