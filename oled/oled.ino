#include <U8g2lib.h>
U8G2_SSD1306_128X64_NONAME_F_SW_I2C oled(U8G2_R0, D4, D3);
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println();

  oled.begin();
  oled.setFont(u8g2_font_wqy16_t_gb2312);
}

void loop() {
  // put your main code here, to run repeatedly:
  oled.clearBuffer();//清空缓冲区
  oled.drawUTF8(0, 30, "中国");
  oled.sendBuffer();//显示缓冲区内容
  delay(1000);
}
