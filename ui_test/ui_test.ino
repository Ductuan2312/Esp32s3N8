#include "Display_ST7789.h"
#include "LVGL_Driver.h"
#include "ui.h"
void setup()
{
          
  LCD_Init();
  Lvgl_Init();
  ui_init();
  
}

void loop()
{
  Timer_Loop();
  delay(5);
}
