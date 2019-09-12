#include <DHT.h>

DHT sensor(D1, DHT11);

void setup() {
  Serial.begin(115200);
  sensor.begin();
}

void loop() {
  float temp = sensor.readTemperature();
  float humi = sensor.readHumidity();
  String line = String(temp) + "," + String(humi);
  Serial.println(line);
  delay(1000);
}
