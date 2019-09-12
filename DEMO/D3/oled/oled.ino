#include <U8g2lib.h>
#include "hqyj.h"
//使用SSD1306控制器
//128x64像素
//无名山寨屏NONAME
//全屏幕缓冲F
//软件实现I2C控制器SW_I2C
//（不旋转-水平方向，SCL=D1，SDA=D2）
U8G2_SSD1306_128X64_NONAME_F_SW_I2C oled(U8G2_R0, D1, D2);

void setup() {
  oled.begin();
  oled.setFont(u8g2_font_wqy16_t_gb2312);//使用自带的16像素文泉驿字体，字符集GB2312
  oled.clearBuffer();//清空缓冲区
  oled.drawUTF8(0, 30, "苟利国家生死以");
  oled.sendBuffer();//显示缓冲区内容
}

void loop() {
  oled.clearBuffer();//清空缓冲区
  oled.drawUTF8(0, 30, "苟利国家生死以");
  //oled.drawBox(3,7,25,15);
  //oled.drawXBM(0, 9, hqyj_width, hqyj_height, (uint8_t*)hqyj_bits);
  oled.sendBuffer();//显示缓冲区内容
}
