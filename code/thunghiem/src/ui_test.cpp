#include "Display_ST7789.h"
#include "LVGL_Driver.h"
#include "ui.h"
#include <OneButton.h>

#define POT_PIN 3     // Chân kết nối với biến trở
#define BOOT_PIN 0    // Chân nút BOOT trên ESP32-S3

// Tạo đối tượng OneButton cho nút BOOT
OneButton bootButton(BOOT_PIN, true, true); // (pin, activeLow, pullupActive)

// Biến theo dõi màn hình hiện tại
bool isScreenOne = true;

// Hàm callback khi nhấn nút BOOT
void switchScreen() {
  if (isScreenOne) {
    // Chuyển sang màn hình 2
    lv_scr_load_anim(ui_screentwo, LV_SCR_LOAD_ANIM_FADE_ON, 300, 0, false);
    isScreenOne = false;
    Serial.println("Switched to Screen Two");
  } else {
    // Chuyển về màn hình 1
    lv_scr_load_anim(ui_screenone, LV_SCR_LOAD_ANIM_FADE_ON, 300, 0, false);
    isScreenOne = true;
    Serial.println("Switched to Screen One");
  }
}

void setup()
{
  Serial.begin(115200);
  delay(1000);  // Đợi Serial khởi động
          
  LCD_Init();
  Lvgl_Init();
  ui_init();
  
  // Cấu hình nút BOOT
  bootButton.attachClick(switchScreen);  // Gắn hàm callback
  bootButton.setDebounceTicks(50);       // Debounce 50ms
  bootButton.setClickTicks(200);         // Click detection 200ms
  
  // Hiển thị màn hình 1 ban đầu
  lv_scr_load(ui_screenone);
  Serial.println("System started - Screen One active");
}

void loop()
{
  // Cập nhật trạng thái nút
  bootButton.tick();
  
  // Đọc giá trị từ biến trở
  int rawValue = analogRead(POT_PIN);        // Đọc giá trị từ biến trở (0 - 4095)
  int percentage = map(rawValue, 0, 4095, 1, 100);  // Quy đổi sang thang 1 - 100

  // Cập nhật màn hình hiện tại
  if (isScreenOne) {
    // Cập nhật màn hình 1
    if(ui_arc1 != NULL) {
      lv_arc_set_value(ui_arc1, percentage);
    }
    
    if(ui_number1 != NULL) {
      char textBuffer[16];
      sprintf(textBuffer, "%d%%", percentage);
      lv_label_set_text(ui_number1, textBuffer);
    }
  } else {
    // Cập nhật màn hình 2
    if(ui_arc2 != NULL) {
      lv_arc_set_value(ui_arc2, percentage);
    }
    
    if(ui_number2 != NULL) {
      char textBuffer[16];
      sprintf(textBuffer, "%d%%", percentage);
      lv_label_set_text(ui_number2, textBuffer);
    }
  }

  Serial.print("Screen: ");
  Serial.print(isScreenOne ? "One" : "Two");
  Serial.print(" | Raw: ");
  Serial.print(rawValue);
  Serial.print(" | Percentage: ");
  Serial.print(percentage);
  Serial.println("%");

  Timer_Loop();
  delay(100);
}