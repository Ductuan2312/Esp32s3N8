#pragma once

#include "lvgl.h"
#include "demos/lv_demos.h"

#include "LVGL_Driver.h"
#include "SD_MMC.h"
#include "Wireless.h"
#include "BAT_Driver.h"
#include "QMI8658.h"

#define EXAMPLE1_LVGL_TICK_PERIOD_MS  1000

extern lv_obj_t * tv;
extern lv_obj_t *Page_panel[50];
extern lv_obj_t *Simulated_panel1[100];
extern size_t Simulated_panel1_Size;


void Backlight_adjustment_event_cb(lv_event_t * e);

void Lvgl_Example1(void);
void LVGL_Backlight_adjustment(uint8_t Backlight);