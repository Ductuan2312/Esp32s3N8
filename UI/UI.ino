#include "Display_ST7789.h"
#include "LVGL_Driver.h"
#include "ui.h"
#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include <Wire.h>
#include "Adafruit_VEML6070.h"

// Cung cấp thông tin cho token generation process
#include "addons/TokenHelper.h"
// Cung cấp thông tin in của RTDB payload
#include "addons/RTDBHelper.h"

// Cấu hình WiFi
#define WIFI_SSID "118/11"     // Thay bằng tên WiFi của bạn
#define WIFI_PASSWORD "@1133557799" // Thay bằng mật khẩu WiFi của bạn

// Cấu hình Firebase
#define FIREBASE_HOST "uv-tracker-app-d2d94-default-rtdb.asia-southeast1.firebasedatabase.app"
#define API_KEY "AIzaSyDJlTjwFjU235bTntpV3aiI_1tvLCgBpKI" // Thay bằng API Key từ Firebase Console
#define DATABASE_URL "https://uv-tracker-app-d2d94-default-rtdb.asia-southeast1.firebasedatabase.app/" // Database URL
#define USER_EMAIL "tuangato247@gmail.com"    // Email Firebase của bạn
#define USER_PASSWORD "123456"               // Password Firebase của bạn

// Cấu hình I2C cho ESP32S3
#define I2C_SDA 6    // Chân SDA
#define I2C_SCL 5    // Chân SCL

// Đối tượng Firebase
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

// Đối tượng UV sensor
Adafruit_VEML6070 uv = Adafruit_VEML6070();

// Biến lưu trữ dữ liệu
float humidity = 0;
float temperature = 0;
float uv_index = 0;        // Thêm biến UV index
float battery_level = 0;   // Thêm biến mức pin
unsigned long lastUpdate = 0;
unsigned long lastUpload = 0;  // Timer cho việc upload dữ liệu
const unsigned long UPDATE_INTERVAL = 5000;  // Cập nhật mỗi 5 giây
const unsigned long UPLOAD_INTERVAL = 10000; // Upload dữ liệu mỗi 10 giây

// Khai báo prototype functions
void connectWiFi();
void connectWiFiWithLoading();
void updateDataFromFirebase();

void setup()
{
  // Khởi tạo Serial để debug
  Serial.begin(115200);
  Serial.println("Khởi động ESP32...");
          
  // Khởi tạo màn hình và UI
  LCD_Init();
  Lvgl_Init();
  ui_init();
  
  // Hiển thị màn hình loading đầu tiên
  lv_scr_load(ui_Screenload);
  Timer_Loop(); // Cập nhật LVGL để hiển thị màn hình
  
  // Khởi tạo I2C với chân tùy chỉnh cho ESP32S3
  Wire.begin(I2C_SDA, I2C_SCL);
  Serial.printf("I2C khởi tạo: SDA=%d, SCL=%d\n", I2C_SDA, I2C_SCL);
  
  // Cập nhật thanh loading - 20%
  lv_bar_set_value(ui_loadtrangthai, 20, LV_ANIM_ON);
  Timer_Loop();
  delay(300);
  
  // Khởi tạo UV sensor VEML6070
  Serial.println("Khởi tạo UV sensor VEML6070...");
  uv.begin(VEML6070_1_T);  // Bỏ if(!...) vì hàm begin() không trả về bool
  
  // Test đọc sensor để kiểm tra kết nối
  delay(100);  // Chờ sensor ổn định
  uint16_t test_uv = uv.readUV();
  if (test_uv == 0xFFFF || test_uv == 0) {
    Serial.println("⚠️ VEML6070 sensor có thể chưa kết nối đúng!");
  } else {
    Serial.printf("✅ VEML6070 sensor đã sẵn sàng! Test value: %d\n", test_uv);
  }
  
  // Cập nhật thanh loading - 40%
  lv_bar_set_value(ui_loadtrangthai, 40, LV_ANIM_ON);
  Timer_Loop();
  delay(300);
  
  // Kết nối WiFi với loading animation
  connectWiFiWithLoading();
  
  // Cấu hình Firebase (theo cách testfirebase)
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;
  
  // Cập nhật thanh loading - 90%
  lv_bar_set_value(ui_loadtrangthai, 90, LV_ANIM_ON);
  Timer_Loop();
  delay(200);
  
  // Khởi tạo Firebase
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
  
  Serial.println("🔥 Firebase đã sẵn sàng!");
  
  // Hoàn thành loading - 100%
  lv_bar_set_value(ui_loadtrangthai, 100, LV_ANIM_ON);
  lv_label_set_text(ui_Label2, "Ready, thank you for waiting!");
  Timer_Loop();
  delay(1000);
  
  // Chuyển sang màn hình chính với hiệu ứng
  lv_scr_load_anim(ui_Screen, LV_SCR_LOAD_ANIM_FADE_OUT, 500, 0, false);
  
  Serial.println("Hoàn tất khởi tạo!");
}

void loop()
{
  // Cập nhật LVGL timer
  Timer_Loop();
  
  // Cập nhật dữ liệu từ Firebase theo interval
  if (millis() - lastUpdate > UPDATE_INTERVAL) {
    updateDataFromFirebase();
    lastUpdate = millis();
  }
  
  // Upload dữ liệu sensor lên Firebase theo interval
  if (millis() - lastUpload > UPLOAD_INTERVAL) {
    uploadSensorDataToFirebase();
    lastUpload = millis();
  }
  
  // Cập nhật UV sensor liên tục (mỗi 2 giây)
  static unsigned long lastUVUpdate = 0;
  if (millis() - lastUVUpdate > 2000) {
    readUVSensor();
    lastUVUpdate = millis();
  }
  
  delay(5);
}

// Hàm kết nối WiFi
void connectWiFi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Đang kết nối WiFi");
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println();
  Serial.println("WiFi đã kết nối!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

void connectWiFiWithLoading() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Đang kết nối WiFi");
  
  int progress = 50; // Bắt đầu từ 50%
  int attempts = 0;
  const int maxAttempts = 60; // Tối đa 30 giây (60 x 500ms)
  
  while (WiFi.status() != WL_CONNECTED && attempts < maxAttempts) {
    delay(500);
    Serial.print(".");
    attempts++;
    
    // Cập nhật thanh loading từ 50% đến 80% trong quá trình kết nối
    if (attempts % 3 == 0) { // Cập nhật mỗi 1.5 giây
      progress = 50 + (attempts * 30) / maxAttempts; // Từ 50% lên 80%
      if (progress > 80) progress = 80;
      lv_bar_set_value(ui_loadtrangthai, progress, LV_ANIM_ON);
      Timer_Loop();
    }
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println();
    Serial.println("WiFi đã kết nối!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
    
    // Cập nhật thanh loading lên 80% khi WiFi kết nối thành công
    lv_bar_set_value(ui_loadtrangthai, 80, LV_ANIM_ON);
    Timer_Loop();
  } else {
    Serial.println();
    Serial.println("Không thể kết nối WiFi!");
    lv_label_set_text(ui_Label2, "WiFi connection failed, retrying...");
    Timer_Loop();
    delay(2000);
    connectWiFiWithLoading(); // Thử lại
  }
}

// Hàm lấy dữ liệu từ Firebase và cập nhật UI
void updateDataFromFirebase() {
  // Kiểm tra kết nối WiFi
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi bị mất kết nối, đang kết nối lại...");
    connectWiFi();
    return;
  }
  
  Serial.println("📥 Đang lấy dữ liệu từ Firebase...");
  
  // Lấy dữ liệu humidity từ Firebase
  if (Firebase.RTDB.getFloat(&fbdo, "/weather/current/humidity")) {
    humidity = fbdo.floatData();
    Serial.print("Humidity: ");
    Serial.println(humidity);
    
    // Cập nhật text lên giao diện LVGL (làm tròn thành số nguyên)
    char humStr[10];
    sprintf(humStr, "%.0f", humidity);  // Đổi từ %.1f thành %.0f để hiển thị số nguyên
    lv_label_set_text(ui_HumNumber, humStr);
    
    // Cập nhật Arc Humidity (0-100%)
    // Sử dụng hàm hỗ trợ để dễ điều chỉnh range
    updateArcValue(ui_ArcHum, humidity, 0.0, 100.0);
    
    Serial.print("Arc Humidity updated to: ");
    Serial.println((int)humidity);
  } else {
    Serial.println("Lỗi khi lấy humidity: " + fbdo.errorReason());
  }
  
  // Lấy dữ liệu temperature từ Firebase
  if (Firebase.RTDB.getFloat(&fbdo, "/weather/current/temperature")) {
    temperature = fbdo.floatData();
    Serial.print("Temperature: ");
    Serial.println(temperature);
    
    // Cập nhật text lên giao diện LVGL (làm tròn thành số nguyên)
    char tempStr[10];
    sprintf(tempStr, "%.0f", temperature);  // Đổi từ %.1f thành %.0f để hiển thị số nguyên
    lv_label_set_text(ui_TempNumber, tempStr);
    
    // Cập nhật Arc Temperature (0-50°C range)
    // Sử dụng hàm hỗ trợ để dễ điều chỉnh range
    updateArcValue(ui_ArcTemp, temperature, 0.0, 50.0);
    
    Serial.print("Arc Temperature updated to: ");
    Serial.println((int)temperature);
  } else {
    Serial.println("Lỗi khi lấy temperature: " + fbdo.errorReason());
  }
}

// Hàm upload dữ liệu sensor lên Firebase
void uploadSensorDataToFirebase() {
  // Kiểm tra kết nối WiFi
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi bị mất kết nối, không thể upload...");
    return;
  }
  
  Serial.println("📤 Đang upload dữ liệu sensor lên Firebase...");
  
  // Đọc UV sensor thực tế (thay thế giả lập)
  readUVSensor();
  
  // Giả lập battery level (bạn có thể thay bằng code đọc ADC thực)
  battery_level = random(200, 1000) / 10.0; // Battery 20-100%
  
  // Upload UV Index lên Firebase
  if (Firebase.RTDB.setFloat(&fbdo, "/sensors/uv_index", uv_index)) {
    Serial.printf("✅ UV Index uploaded to Firebase: %.1f\n", uv_index);
  } else {
    Serial.printf("❌ Lỗi upload UV Index: %s\n", fbdo.errorReason().c_str());
  }
  
  // Upload Battery Level
  if (Firebase.RTDB.setFloat(&fbdo, "/sensors/battery_level", battery_level)) {
    Serial.printf("✅ Battery Level uploaded: %.1f%%\n", battery_level);
  } else {
    Serial.printf("❌ Lỗi upload Battery Level: %s\n", fbdo.errorReason().c_str());
  }
  
  // Upload timestamp
  unsigned long timestamp = millis();
  if (Firebase.RTDB.setInt(&fbdo, "/sensors/last_update", timestamp)) {
    Serial.printf("✅ Timestamp uploaded: %lu\n", timestamp);
  } else {
    Serial.printf("❌ Lỗi upload Timestamp: %s\n", fbdo.errorReason().c_str());
  }
}

// Hàm đọc UV sensor và cập nhật UI
void readUVSensor() {
  // Đọc giá trị raw từ VEML6070
  uint16_t uv_raw = uv.readUV();
  
  // Chuyển đổi từ giá trị raw sang UV Index (0-15)
  // VEML6070 trả về giá trị 0-65535, ta cần map sang 0-15
  // Theo datasheet: giá trị ~2300 tương đương UV Index 11 (very high)
  uv_index = map(uv_raw, 0, 2300, 0, 150) / 10.0; // Chia 10 để có thập phân
  
  // Giới hạn trong khoảng 0-15
  if (uv_index > 15.0) uv_index = 15.0;
  if (uv_index < 0.0) uv_index = 0.0;
  
  Serial.printf("UV Raw: %d, UV Index: %.1f\n", uv_raw, uv_index);
  
  // Cập nhật lên UI
  char uvStr[10];
  sprintf(uvStr, "%.1f", uv_index);
  lv_label_set_text(ui_UVNumber, uvStr);
  
  // Cập nhật Arc UV (0-15 range)
  updateArcValue(ui_ArcUv, uv_index, 0.0, 15.0);
  
  // 🎨 CẬP NHẬT MÀU SẮC ARC THEO UV INDEX
  updateArcUVColor(uv_index);
  
  // 📝 CẬP NHẬT TEXT CẢNH BÁO
  updateUVWarningText(uv_index);
  
  Serial.printf("✅ UV Index updated on UI: %.1f\n", uv_index);
}

// Hàm hỗ trợ để cập nhật Arc với range tùy chỉnh
void updateArcValue(lv_obj_t* arc, float value, float min_val, float max_val) {
  // Map giá trị từ range thực tế sang 0-100 cho Arc
  int arcValue = map((int)(value * 10), (int)(min_val * 10), (int)(max_val * 10), 0, 100);
  // Giới hạn trong khoảng 0-100
  arcValue = constrain(arcValue, 0, 100);
  lv_arc_set_value(arc, arcValue);
}

// 🎨 Hàm thay đổi màu sắc Arc UV theo mức độ UV Index
void updateArcUVColor(float uv_index) {
  uint32_t color_hex;
  
  if (uv_index < 3.0) {
    // Low (0-2): Màu xanh lá cây - An toàn
    color_hex = 0x00FF00;  // Xanh lá
  } else if (uv_index < 6.0) {
    // Moderate (3-5): Màu vàng - Cẩn thận
    color_hex = 0xFFFF00;  // Vàng
  } else if (uv_index < 8.0) {
    // High (6-7): Màu cam - Nguy hiểm
    color_hex = 0xFF8000;  // Cam
  } else if (uv_index < 11.0) {
    // Very High (8-10): Màu đỏ - Rất nguy hiểm
    color_hex = 0xFF0000;  // Đỏ
  } else {
    // Extreme (11+): Màu tím - Cực kỳ nguy hiểm
    color_hex = 0x8000FF;  // Tím
  }
  
  // Áp dụng màu cho Arc indicator (thanh tiến trình)
  lv_obj_set_style_arc_color(ui_ArcUv, lv_color_hex(color_hex), LV_PART_INDICATOR | LV_STATE_DEFAULT);
  
  // Áp dụng màu cho Arc knob (nút tròn)
  lv_obj_set_style_bg_color(ui_ArcUv, lv_color_hex(color_hex), LV_PART_KNOB | LV_STATE_DEFAULT);
  
  // Debug output
  Serial.printf("🎨 UV Index: %.1f - Màu Arc: 0x%06X\n", uv_index, color_hex);
}

// 📝 Hàm cập nhật text cảnh báo theo mức độ UV
void updateUVWarningText(float uv_index) {
  if (uv_index < 3.0) {
    // Low - An toàn
    lv_label_set_text(ui_canhbao, "Safe - No protection needed\nEnjoy outdoor activities");
    lv_obj_set_style_text_color(ui_canhbao, lv_color_hex(0x00FF00), LV_PART_MAIN | LV_STATE_DEFAULT);
  } else if (uv_index < 6.0) {
    // Moderate - Cẩn thận
    lv_label_set_text(ui_canhbao, "Moderate - Wear sunglasses\nSeek shade during midday");
    lv_obj_set_style_text_color(ui_canhbao, lv_color_hex(0xFFFF00), LV_PART_MAIN | LV_STATE_DEFAULT);
  } else if (uv_index < 8.0) {
    // High - Cần bảo vệ
    lv_label_set_text(ui_canhbao, "High - SPF 30+ sunscreen\nWear protective clothing");
    lv_obj_set_style_text_color(ui_canhbao, lv_color_hex(0xFF8000), LV_PART_MAIN | LV_STATE_DEFAULT);
  } else if (uv_index < 11.0) {
    // Very High - Rất nguy hiểm
    lv_label_set_text(ui_canhbao, "Very High - SPF 50+ sunscreen\nMinimize sun exposure");
    lv_obj_set_style_text_color(ui_canhbao, lv_color_hex(0xFF0000), LV_PART_MAIN | LV_STATE_DEFAULT);
  } else {
    // Extreme - Cực kỳ nguy hiểm
    lv_label_set_text(ui_canhbao, "Extreme - Avoid sun exposure\nStay indoors 10AM-4PM");
    lv_obj_set_style_text_color(ui_canhbao, lv_color_hex(0x8000FF), LV_PART_MAIN | LV_STATE_DEFAULT);
  }
}
