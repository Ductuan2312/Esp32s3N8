// Test UV Sensor VEML6070 - Code riêng để kiểm tra
#include <Wire.h>
#include "Adafruit_VEML6070.h"

// Cấu hình I2C cho ESP32S3
#define I2C_SDA 6    // Chân SDA
#define I2C_SCL 5    // Chân SCL

// Đối tượng UV sensor
Adafruit_VEML6070 uv = Adafruit_VEML6070();

void setup() {
  // Khởi tạo Serial
  Serial.begin(115200);
  Serial.println("=== TEST UV SENSOR VEML6070 ===");
  
  // Khởi tạo I2C với chân tùy chỉnh cho ESP32S3
  Wire.begin(I2C_SDA, I2C_SCL);
  Serial.printf("I2C khởi tạo: SDA=%d, SCL=%d\n", I2C_SDA, I2C_SCL);
  
  // Khởi tạo UV sensor VEML6070
  Serial.println("Khởi tạo UV sensor VEML6070...");
  uv.begin(VEML6070_1_T);  // Integration time: 1T (125ms)
  
  delay(500);  // Chờ sensor ổn định
  
  // Test đầu tiên
  uint16_t test_value = uv.readUV();
  Serial.printf("Test đọc đầu tiên: %d\n", test_value);
  
  if (test_value == 0xFFFF) {
    Serial.println("❌ Sensor trả về 0xFFFF - có thể chưa kết nối!");
  } else if (test_value == 0) {
    Serial.println("⚠️ Sensor trả về 0 - kiểm tra nguồn sáng UV!");
  } else {
    Serial.println("✅ Sensor hoạt động bình thường!");
  }
  
  Serial.println("\n--- Bắt đầu đo liên tục ---");
  Serial.println("Raw Value | UV Index | Mức độ");
  Serial.println("----------|----------|--------");
}

void loop() {
  // Đọc giá trị raw từ VEML6070
  uint16_t uv_raw = uv.readUV();
  
  // Chuyển đổi sang UV Index (0-15)
  float uv_index = convertToUVIndex(uv_raw);
  
  // Xác định mức độ UV
  String uv_level = getUVLevel(uv_index);
  
  // In ra Serial Monitor
  Serial.printf("%9d | %8.1f | %s\n", uv_raw, uv_index, uv_level.c_str());
  
  delay(2000);  // Đo mỗi 2 giây
}

// Hàm chuyển đổi raw value sang UV Index
float convertToUVIndex(uint16_t raw_value) {
  // Theo datasheet VEML6070 và thực nghiệm:
  // Integration time 1T (125ms): 
  // - 0-500: UV Index 0-2 (Low)
  // - 500-1200: UV Index 3-5 (Moderate) 
  // - 1200-2300: UV Index 6-7 (High)
  // - 2300-3000: UV Index 8-10 (Very High)
  // - >3000: UV Index 11+ (Extreme)
  
  float uv_index;
  
  if (raw_value <= 500) {
    // Low: 0-2
    uv_index = map(raw_value, 0, 500, 0, 20) / 10.0;
  } else if (raw_value <= 1200) {
    // Moderate: 3-5  
    uv_index = map(raw_value, 500, 1200, 30, 50) / 10.0;
  } else if (raw_value <= 2300) {
    // High: 6-7
    uv_index = map(raw_value, 1200, 2300, 60, 70) / 10.0;
  } else if (raw_value <= 3000) {
    // Very High: 8-10
    uv_index = map(raw_value, 2300, 3000, 80, 100) / 10.0;
  } else {
    // Extreme: 11+
    uv_index = map(raw_value, 3000, 4000, 110, 150) / 10.0;
    if (uv_index > 15.0) uv_index = 15.0;
  }
  
  return uv_index;
}

// Hàm xác định mức độ UV
String getUVLevel(float uv_index) {
  if (uv_index < 3) {
    return "Low (Thấp)";
  } else if (uv_index < 6) {
    return "Moderate (Vừa)";
  } else if (uv_index < 8) {
    return "High (Cao)";
  } else if (uv_index < 11) {
    return "Very High (Rất cao)";
  } else {
    return "Extreme (Cực cao)";
  }
}