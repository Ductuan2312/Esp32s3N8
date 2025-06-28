#include <Arduino.h>
#include <FastLED.h>

// RGB LED (WS2812B) - theo code mẫu nhà sản xuất
#define PIN_NEOPIXEL 38
#define NUM_LEDS 1

// Mảng LED
CRGB leds[NUM_LEDS];

// Hàm set màu (theo style nhà sản xuất)
void Set_Color(uint8_t Red, uint8_t Green, uint8_t Blue) {
  leds[0] = CRGB(Red, Green, Blue);
  FastLED.show();
}

// Hàm chuyển màu mượt (RGB Loop)
void RGB_Loop(uint16_t Waiting) {
  // Chuyển từ đỏ sang xanh lá
  for(int i = 0; i <= 255; i++) {
    Set_Color(255 - i, i, 0);
    delay(Waiting);
  }
  
  // Chuyển từ xanh lá sang xanh dương  
  for(int i = 0; i <= 255; i++) {
    Set_Color(0, 255 - i, i);
    delay(Waiting);
  }
  
  // Chuyển từ xanh dương về đỏ
  for(int i = 0; i <= 255; i++) {
    Set_Color(i, 0, 255 - i);
    delay(Waiting);
  }
}

void setup() {
  // Khởi tạo USB CDC Serial
  Serial.begin(115200);
  
  // Đợi USB CDC kết nối (quan trọng cho ESP32-S3)
  unsigned long start = millis();
  while (!Serial && (millis() - start) < 10000) {
    delay(100); // Đợi USB CDC ready
  }
  
  delay(1000); // Thêm delay để đảm bảo ổn định
  
  // Test Serial với USB CDC
  Serial.println();
  Serial.println("==============================================");
  Serial.println("ESP32-S3-LCD-1.47B USB CDC Serial Test");
  Serial.println("==============================================");
  Serial.print("Boot time: ");
  Serial.print(millis());
  Serial.println(" ms");
  Serial.print("USB CDC ready: ");
  Serial.println(Serial ? "YES" : "NO");
  Serial.flush();
  Serial.println("==============================================");
  Serial.print("⏰ Boot time: ");
  Serial.print(millis());
  Serial.println(" ms");
  
  // Khởi tạo FastLED
  Serial.println("🔧 Initializing FastLED...");
  FastLED.addLeds<WS2812B, PIN_NEOPIXEL, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(100);
  
  // Test LED
  Serial.println("💡 Testing LED...");
  Set_Color(255, 0, 0); // Đỏ
  delay(500);
  Set_Color(0, 255, 0); // Xanh lá
  delay(500);
  Set_Color(0, 0, 255); // Xanh dương
  delay(500);
  Set_Color(0, 0, 0);   // Tắt
  
  Serial.println("✅ Setup complete!");
  Serial.print("🌈 NeoPixel Pin: GPIO ");
  Serial.println(PIN_NEOPIXEL);
  Serial.println("🎨 Starting color effects...");
  Serial.println("==============================================");
  Serial.flush(); // Đảm bảo tất cả được gửi
}

void loop() {
  Serial.println("🌈 Smooth RGB Color Loop");
  
  // Hiệu ứng chuyển màu mượt (chậm)
  RGB_Loop(5);
  
  Serial.println("✨ Fast Rainbow Effect");
  
  // Hiệu ứng chuyển màu nhanh
  RGB_Loop(1);
  
  Serial.println("💙 Breathing Effect");
  
  // Hiệu ứng thở (fade in/out) màu xanh dương
  for(int brightness = 0; brightness <= 255; brightness += 5) {
    Set_Color(0, 0, brightness);
    delay(20);
  }
  for(int brightness = 255; brightness >= 0; brightness -= 5) {
    Set_Color(0, 0, brightness);
    delay(20);
  }
  
  Serial.println("---");
  delay(1000);
}