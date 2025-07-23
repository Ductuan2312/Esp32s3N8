#pragma once
#include <Arduino.h>
#include <SPI.h>
// Hoán đổi kích thước để hiển thị ngang: từ 172x320 thành 320x172
#define LCD_WIDTH   320 //LCD width - đã đổi từ 172 thành 320 cho hiển thị ngang
#define LCD_HEIGHT  172 //LCD height - đã đổi từ 320 thành 172 cho hiển thị ngang
// Code cũ (hiển thị dọc):
// #define LCD_WIDTH   172 //LCD width
// #define LCD_HEIGHT  320 //LCD height

#define SPIFreq                        80000000
#define EXAMPLE_PIN_NUM_MISO           -1
#define EXAMPLE_PIN_NUM_MOSI           45
#define EXAMPLE_PIN_NUM_SCLK           40
#define EXAMPLE_PIN_NUM_LCD_CS         42
#define EXAMPLE_PIN_NUM_LCD_DC         41
#define EXAMPLE_PIN_NUM_LCD_RST        39
#define EXAMPLE_PIN_NUM_BK_LIGHT       46
#define Frequency       1000                    // PWM frequencyconst 
#define Resolution      10                         
#define Backlight_MAX   100     

#define VERTICAL   0
#define HORIZONTAL 1  // Đã set = 1 để kích hoạt chế độ hiển thị ngang

// Điều chỉnh offset cho hiển thị ngang
#define Offset_X 0   // Đã đổi từ 34 thành 0 cho hiển thị ngang
#define Offset_Y 34  // Đã đổi từ 0 thành 34 cho hiển thị ngang
// Code cũ (hiển thị dọc):
// #define Offset_X 34
// #define Offset_Y 0

extern uint8_t LCD_Backlight;

void LCD_SetCursor(uint16_t x1, uint16_t y1, uint16_t x2,uint16_t y2);

void LCD_Init(void);
void LCD_SetCursor(uint16_t Xstart, uint16_t Ystart, uint16_t Xend, uint16_t  Yend);
void LCD_addWindow(uint16_t Xstart, uint16_t Ystart, uint16_t Xend, uint16_t Yend,uint16_t* color);

void Backlight_Init(void);
void Set_Backlight(uint8_t Light);
