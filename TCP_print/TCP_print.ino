#include <U8g2lib.h>
#include <ESP8266WiFi.h>

WiFiServer tcpserver(666);
U8G2_SSD1306_128X64_NONAME_F_SW_I2C oled(U8G2_R0, D4, D3);
void AP_set(){
  WiFi.mode(WIFI_AP);
  WiFi.softAP("@", "12345678");
  Serial.println(WiFi.softAPIP().toString());
  tcpserver.begin();
}
void TCP_print(){
  WiFiClient tcpclient = tcpserver.available();
  while (tcpclient.connected())
  {
    if (tcpclient.available())
    {
    String cmd = tcpclient.readStringUntil('\n');
    cmd.trim();
    oled.clearBuffer();
    oled.drawUTF8(0, 30, cmd.c_str());
    oled.sendBuffer();
    }
    delay(1000);
  }
}
void OLED_set(){
  oled.begin();
  oled.setFont(u8g2_font_wqy16_t_gb2312);
}
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println();

  pinMode(16, OUTPUT);
  digitalWrite(16, LOW);
  OLED_set();
  AP_set();
}

void loop() {
  // put your main code here, to run repeatedly:
  TCP_print();
}
