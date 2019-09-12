#include <U8g2lib.h> 
//#include "hqyj.h"
//软件实现I2C控制器(不旋转-水平方向，scl=D1,SDA=D2)
U8G2_SSD1306_128X64_NONAME_F_SW_I2C oled(U8G2_R0, D1, D2);
void setup() {
  // put your setup code here, to run once:
  oled.begin();
  oled.setFont(u8g2_font_wqy16_t_gb2312);
}

void loop() {
  // put your main code here, to run repeatedly:
  oled.clearBuffer();
  oled.drawUTF8(0, 30, "中国")；
  oled.sendBuffer();
}
