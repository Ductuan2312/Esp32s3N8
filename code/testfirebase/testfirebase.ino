#include "WiFiHelper.h"
#include <Firebase_ESP_Client.h>
#include <time.h>

// Firebase
#define API_KEY "AIzaSyCI-WK9TOcYAKxOarCR5px2wm-ndSYsGhs"
#define DATABASE_URL "https://test1-9a697-default-rtdb.firebaseio.com/"
#define USER_EMAIL "tuangato147@gmail.com"
#define USER_PASSWORD "tuan123"

// LED GPIO
#define LED1_PIN 2
#define LED2_PIN 4
#define BOOT_PIN 0

// Global objects
WiFiHelper wifi;
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

bool lastLED1 = false;
bool lastLED2 = false;

void setup() {
    Serial.begin(115200);
    Serial.println("🚀 ESP32 Firebase + WiFiManager");
    
    // Cấu hình GPIO
    pinMode(LED1_PIN, OUTPUT);
    pinMode(LED2_PIN, OUTPUT);
    pinMode(BOOT_PIN, INPUT_PULLUP);
    
    // Khởi tạo WiFi
    if (!wifi.begin("ESP32-Setup", "12345678")) {
        ESP.restart();
    }
    
    // Cấu hình thời gian
    configTime(0, 0, "pool.ntp.org", "time.nist.gov");
    Serial.print("⏰ Đồng bộ thời gian");
    while (time(nullptr) < 100000) {
        delay(500);
        Serial.print(".");
    }
    Serial.println(" ✅");
    
    // Cấu hình Firebase
    config.api_key = API_KEY;
    config.database_url = DATABASE_URL;
    auth.user.email = USER_EMAIL;
    auth.user.password = USER_PASSWORD;
    
    Firebase.begin(&config, &auth);
    Firebase.reconnectWiFi(true);
    
    Serial.println("🔥 Firebase đã sẵn sàng!");
    Serial.println("💡 Giữ nút BOOT 2s để reset WiFi");
}

void loop() {
    // Kiểm tra nút BOOT để reset WiFi
    static unsigned long buttonPressTime = 0;
    static bool buttonPressed = false;
    
    bool currentButtonState = digitalRead(BOOT_PIN) == LOW;
    
    if (currentButtonState && !buttonPressed) {
        // Bắt đầu nhấn nút
        buttonPressTime = millis();
        buttonPressed = true;
        Serial.println("🔘 Nhấn nút BOOT...");
    } 
    else if (!currentButtonState && buttonPressed) {
        // Thả nút
        buttonPressed = false;
        unsigned long pressDuration = millis() - buttonPressTime;
        
        if (pressDuration >= 2000) {
            wifi.resetSettings();
        } else {
            Serial.println("🔘 Thả nút (chưa đủ 2s)");
        }
    }
    
    // Kiểm tra kết nối WiFi
    if (!wifi.isConnected()) {
        Serial.println("⚠️ Mất kết nối WiFi!");
        return;
    }
    
    // Đọc trạng thái LED1 từ Firebase
    if (Firebase.RTDB.getBool(&fbdo, "/led/status")) {
        bool led1State = fbdo.boolData();
        if (led1State != lastLED1) {
            digitalWrite(LED1_PIN, led1State);
            Serial.print("[LED1] ");
            Serial.println(led1State ? "ON" : "OFF");
            lastLED1 = led1State;
        }
    } else {
        Serial.print("[LED1 ❌] ");
        Serial.println(fbdo.errorReason());
    }

    // Đọc trạng thái LED2 từ Firebase
    if (Firebase.RTDB.getBool(&fbdo, "/led2/status")) {
        bool led2State = fbdo.boolData();
        if (led2State != lastLED2) {
            digitalWrite(LED2_PIN, led2State);
            Serial.print("[LED2] ");
            Serial.println(led2State ? "ON" : "OFF");
            lastLED2 = led2State;
        }
    } else {
        Serial.print("[LED2 ❌] ");
        Serial.println(fbdo.errorReason());
    }

    delay(500);
}
