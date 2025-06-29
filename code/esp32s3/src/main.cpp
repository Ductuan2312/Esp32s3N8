#include <Arduino.h>
#include <TFT_eSPI.h>
#include <SPI.h>

TFT_eSPI tft = TFT_eSPI();

void setup() {
    Serial.begin(115200);
    delay(2000); // Đợi Serial ready
    
    Serial.println("=== ESP32-S3 Display Test ===");
    
    // Khởi tạo chân backlight theo pin mapping mới
    pinMode(46, OUTPUT);     // TFT_BL = GPIO46
    digitalWrite(46, HIGH);  // Bật backlight
    Serial.println("Backlight ON (GPIO46)");
    
    // Khởi tạo chân reset theo pin mapping mới
    pinMode(39, OUTPUT);     // TFT_RST = GPIO39
    digitalWrite(39, LOW);   // Reset LOW
    delay(50);
    digitalWrite(39, HIGH);  // Reset HIGH
    delay(50);
    Serial.println("Reset done (GPIO39)");
    
    // Khởi tạo TFT
    Serial.println("Initializing TFT...");
    tft.init();
    Serial.println("TFT init done");
    
    tft.setRotation(0);      // Portrait mode
    Serial.printf("Display size: %dx%d\n", tft.width(), tft.height());
    
    // Test các màu cơ bản
    Serial.println("Testing colors...");
    
    // Màu đỏ
    tft.fillScreen(TFT_RED);
    Serial.println("RED screen");
    delay(2000);
    
    // Màu xanh lá
    tft.fillScreen(TFT_GREEN);
    Serial.println("GREEN screen");
    delay(2000);
    
    // Màu xanh dương
    tft.fillScreen(TFT_BLUE);
    Serial.println("BLUE screen");
    delay(2000);
    
    // Màu trắng với text
    tft.fillScreen(TFT_WHITE);
    tft.setTextColor(TFT_BLACK, TFT_WHITE);
    tft.setTextFont(2);
    tft.drawString("ESP32-S3", 10, 10);
    tft.drawString("Waveshare", 10, 30);
    tft.drawString("Display Test", 10, 50);
    Serial.println("WHITE screen with text");
    delay(2000);
    
    // Màu đen với text trắng
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextFont(4);
    tft.drawString("WORKING!", 10, 100);
    Serial.println("Test completed - display should show WORKING!");
}

void loop() {
    // Nhấp nháy màn hình mỗi 3 giây
    static unsigned long lastTime = 0;
    if (millis() - lastTime > 3000) {
        lastTime = millis();
        
        // Đổi màu nền
        static int colorIndex = 0;
        uint16_t colors[] = {TFT_BLACK, TFT_RED, TFT_GREEN, TFT_BLUE, TFT_YELLOW, TFT_CYAN, TFT_MAGENTA};
        
        tft.fillScreen(colors[colorIndex]);
        tft.setTextColor(TFT_WHITE, colors[colorIndex]);
        tft.setTextFont(2);
        tft.drawString("Color Test " + String(colorIndex), 10, 10);
        
        colorIndex = (colorIndex + 1) % 7;
        Serial.println("Color changed: " + String(colorIndex));
    }
    
    delay(100);
}
