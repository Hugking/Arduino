#include <DHT.h>
DHT sensor(D1, DHT11);
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  sensor.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  float temp = sensor.readTemperature();
  float humi = sensor.readHumidity();
  String line = String(temp)+","+String(humi);
  Serial.println(line);
}
