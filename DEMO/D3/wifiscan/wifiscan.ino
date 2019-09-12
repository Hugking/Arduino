#include <ESP8266WiFi.h>

void setup() {
  Serial.begin(115200);
  Serial.println();
  
  int n = WiFi.scanNetworks();
  for(int i; i < n; i++)
  {
    String line = WiFi.SSID(i) + ", 信道：" + WiFi.channel(i);
    Serial.println(line);
  }
}

void loop() {
  // put your main code here, to run repeatedly:

}
