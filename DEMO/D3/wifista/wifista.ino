#include <ESP8266WiFi.h>
#include <U8g2lib.h>

U8G2_SSD1306_128X64_NONAME_F_SW_I2C oled(U8G2_R0, D1, D2);

void setup() {
  Serial.begin(115200);
  Serial.println();
  oled.begin();
  oled.setFont(u8g2_font_wqy16_t_gb2312);
  
  WiFi.mode(WIFI_STA);
  WiFi.begin("Taro iPhone", "d7pe1uay97mfu");
  while(!WiFi.isConnected())
  {
    delay(100);
  }
  Serial.println("WiFi is connected");
  Serial.println(WiFi.localIP().toString());
  configTime(8 * 3600, 0, "pool.ntp.org");
}

void loop() {
  time_t now = time(nullptr);
  struct tm * timeinfo = localtime(&now);
  char buffer[80];
  strftime(buffer, 80, "%H:%M:%S", timeinfo);
  
  oled.clearBuffer();//清空缓冲区
  oled.drawUTF8(0, 30, buffer);
  oled.sendBuffer();//显示缓冲区内容
  delay(1000);
}
