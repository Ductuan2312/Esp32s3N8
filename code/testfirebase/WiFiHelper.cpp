#include "WiFiHelper.h"

bool WiFiHelper::begin(const char* apName, const char* apPassword) {
    WiFi.mode(WIFI_STA);
    
    Serial.println("🔍 Đang tìm WiFi đã lưu...");
    
    // Thiết lập WiFiManager
    wm.setDebugOutput(false); // Tắt debug để gọn
    wm.setConfigPortalTimeout(180); // 3 phút timeout
    
    // Tự động kết nối hoặc tạo AP
    bool res = wm.autoConnect(apName, apPassword);
    
    if (!res) {
        Serial.println("❌ Không thể kết nối WiFi");
        return false;
    } else {
        Serial.println("✅ Đã kết nối WiFi thành công!");
        printStatus();
        return true;
    }
}

void WiFiHelper::resetSettings() {
    Serial.println("🗑️ Reset WiFi settings!");
    wm.resetSettings();
    delay(1000);
    ESP.restart();
}

bool WiFiHelper::isConnected() {
    return WiFi.status() == WL_CONNECTED;
}

void WiFiHelper::printStatus() {
    if (isConnected()) {
        Serial.print("📡 SSID: ");
        Serial.println(WiFi.SSID());
        Serial.print("📶 IP: ");
        Serial.println(WiFi.localIP());
        Serial.print("📊 Signal: ");
        Serial.print(WiFi.RSSI());
        Serial.println(" dBm");
    } else {
        Serial.println("⚠️ WiFi không kết nối");
    }
}
