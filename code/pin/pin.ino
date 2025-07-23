// Thêm include và khai báo
#include "BAT_Driver.h"

void setup() {
  Serial.begin(115200); // Khởi động Serial Monitor
  BAT_Init();           // Khởi động đo pin
}

void loop() {
  float volts = BAT_Get_Volts();
  Serial.print("BAT voltage: ");
  Serial.print(volts, 2);
  Serial.println(" V");
  delay(1000); // Đo mỗi giây
}
