const int lm35Pin = 5; // GPIO5 của ESP32-S3
const int numSamples = 10; // số mẫu trung bình

void setup() {
  Serial.begin(9600);
}

void loop() {
  long total = 0;

  // Lấy mẫu nhiều lần để giảm nhiễu
  for (int i = 0; i < numSamples; i++) {
    total += analogRead(lm35Pin);
    delay(10); // nhỏ để ổn định đọc ADC
  }

  float average = total / (float)numSamples;
  float voltage = (average / 4095.0) * 3.3; // 12-bit ADC, 3.3V
  float temperatureC = voltage / 0.01;

  Serial.print("Nhiet do: ");
  Serial.print(temperatureC, 2);
  Serial.println(" °C");

  delay(1000);
}
