#pragma once

#include <lvgl.h>
#include <lv_conf.h>
#include <esp_heap_caps.h>
#include "Display_ST7789.h"

// Cập nhật kích thước LVGL để đồng bộ với màn hình ngang
#define LVGL_WIDTH    (LCD_WIDTH )  // Bây giờ là 320 (đã hoán đổi từ 172)
#define LVGL_HEIGHT   LCD_HEIGHT    // Bây giờ là 172 (đã hoán đổi từ 320)
#define LVGL_BUF_LEN  (LVGL_WIDTH * LVGL_HEIGHT / 20)
// Code cũ (hiển thị dọc): LVGL_WIDTH = 172, LVGL_HEIGHT = 320

#define EXAMPLE_LVGL_TICK_PERIOD_MS  5


void Lvgl_print(const char * buf);
void Lvgl_Display_LCD( lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p ); // Displays LVGL content on the LCD.    This function implements associating LVGL data to the LCD screen
void Lvgl_Touchpad_Read( lv_indev_drv_t * indev_drv, lv_indev_data_t * data );                // Read the touchpad
void example_increase_lvgl_tick(void *arg);

void Lvgl_Init(void);
void Timer_Loop(void);
