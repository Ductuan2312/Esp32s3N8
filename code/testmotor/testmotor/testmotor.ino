#define MOTOR_PWM_PIN 6

void setup() {
  Serial.begin(115200);
  pinMode(MOTOR_PWM_PIN, OUTPUT);

  // Thiết lập PWM ổn định (nếu core hỗ trợ)
  analogWriteFrequency(MOTOR_PWM_PIN, 1000);  // 1kHz
  analogWriteResolution(MOTOR_PWM_PIN, 8);    // 8-bit: 0–255
}

void loop() {
  // Rung mạnh
  analogWrite(MOTOR_PWM_PIN, 255);
  delay(500);  // Rung 100ms

  // Ngắt
  analogWrite(MOTOR_PWM_PIN, 0);
  delay(150);  // Dừng 150ms

  // Lặp lại tạo "ụt ụt ụt..."
}
