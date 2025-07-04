#ifndef WIFI_HELPER_H
#define WIFI_HELPER_H

#include <WiFi.h>
#include <WiFiManager.h>

class WiFiHelper {
private:
    WiFiManager wm;
    
public:
    // Khởi tạo WiFi với autoConnect
    bool begin(const char* apName = "ESP32-Setup", const char* apPassword = "12345678");
    
    // Reset WiFi settings
    void resetSettings();
    
    // Kiểm tra kết nối
    bool isConnected();
    
    // In thông tin WiFi
    void printStatus();
};

#endif
