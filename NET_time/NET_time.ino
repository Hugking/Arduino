#include <ESP8266WiFi.h>
#include <U8g2lib.h>

U8G2_SSD1306_128X64_NONAME_F_SW_I2C oled(U8G2_R0, D4, D3);
void NET_time_show(){
  time_t now = time(nullptr);
  struct tm * timeinfo = localtime(&now);
  char buffer[80];
  strftime(buffer, 80, "%H:%M:%S", timeinfo);
  oled.clearBuffer();//清空缓冲区
  oled.drawUTF8(0, 30, buffer);
  oled.sendBuffer();//显示缓冲区内容
  delay(1000);
}
void OLED_set(){
  oled.begin();
  oled.setFont(u8g2_font_wqy16_t_gb2312);
}
void STA_set(){
  WiFi.mode(WIFI_STA);
  WiFi.begin("AK", "12345678");
  oled.clearBuffer();
  oled.drawUTF8(0, 30, "连接中");
  int i = 35 ;
  while(!WiFi.isConnected()){
    delay(300);
    Serial.print(".");
    i = i + 3;
    i % 2 == 0 ? digitalWrite(16, LOW):digitalWrite(16, HIGH);
    oled.drawUTF8(i, 30, " . ");
    oled.sendBuffer();
    if (i>45){
        oled.clearBuffer();
        oled.drawUTF8(0, 30, "连接中");
        i = 35;
      }
    }
  Serial.println("WiFi is connected");
  Serial.println(WiFi.localIP().toString());
  digitalWrite(16, LOW);
  oled.clearBuffer();
  oled.drawUTF8(0, 30, "连接成功！");
  oled.sendBuffer();
  delay(3000);
}
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println();

  pinMode(16, OUTPUT);
  digitalWrite(16, LOW);
  
  OLED_set();
  STA_set();
  configTime(8 * 3600, 0, "pool.ntp.org");
}

void loop() {
  // put your main code here, to run repeatedly:
  NET_time_show();
}
