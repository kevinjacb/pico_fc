#include "gui.h"
#include <Adafruit_ST7735.h>
#include <Adafruit_GFX.h>

GUI::GUI(Adafruit_ST7735 *tft):tft(tft){
  tft -> initR(INITR_BLACKTAB);
  tft -> fillScreen(ST77XX_BLACK);
  tft -> setRotation(1);
}

void GUI::displayInfo(String data, int text_size,int x, int y, int16_t color,int16_t bg, bool clr,bool shift){
  if (x != -1 && y != -1)
    tft -> setCursor(x,y);
  if (clr)
    tft -> fillScreen(ST77XX_BLACK);
  tft -> setTextColor(color,bg);
  tft -> setTextSize(text_size);
  if(shift)
    tft -> println(data);
  else
    tft -> print(data);
}


int GUI::createMenu(String items[],int count){
  GUI::displayInfo("MENU",2,tw/3,10,ST77XX_YELLOW);
  for(int i = 0,y_offset = 0; i < count; i++,y_offset += incr_y)
    GUI::displayInfo("-> "+items[i],1,tw/10,init_y+y_offset,ST77XX_CYAN);
  return 1;
}

int GUI::highlightItem(String items[],int item,int prev_item,int item_count){
  if(prev_item != -1)
    GUI::displayInfo("-> "+items[prev_item],1,tw/10,init_y+incr_y*prev_item,ST77XX_CYAN,ST77XX_BLACK,false,false);
  GUI::displayInfo("-> "+items[item],1,tw/10,init_y+incr_y*item,ST77XX_BLACK,ST77XX_WHITE,false,false);
  return 1;
}

int GUI::selectItem(int item){
  //TODO
  return 1;
}
