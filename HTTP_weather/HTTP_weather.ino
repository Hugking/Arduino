#include <U8g2lib.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

U8G2_SSD1306_128X64_NONAME_F_SW_I2C oled(U8G2_R0, D4, D3);
WiFiClient tcp;
String weather_info;
String ip;
void HTTP_weather(){
  HTTPClient http;
  http.begin(tcp, "http://www.nmc.cn/f/rest/weather/57171");
  if (http.GET() == 200){
    Serial.println(http.getString());
    //https://arduinojson.org/v6/assistant/
    const size_t capacity = JSON_ARRAY_SIZE(1) + JSON_ARRAY_SIZE(2) + 8*JSON_OBJECT_SIZE(2) + 4*JSON_OBJECT_SIZE(3) + 3*JSON_OBJECT_SIZE(4) + JSON_OBJECT_SIZE(5) + 530;
    DynamicJsonDocument doc(capacity);
    deserializeJson(doc, http.getString());
    const char* info = doc[0]["detail"][0]["day"]["weather"]["info"];
    const char* temp = doc[0]["detail"][0]["day"]["weather"]["temperature"];
    const char* city = doc[0]["station"]["city"];
    weather_info = city + String("，") + info + "，" + temp + "℃";
    Serial.println(weather_info);
 }
 http.end();
}
void STA_set(){
  WiFi.mode(WIFI_STA);
  WiFi.begin("AK", "12345678");
  oled.clearBuffer();
  oled.drawUTF8(0, 20, "连接中");
  int i = 35 ;
  while(!WiFi.isConnected()){
    delay(300);
    Serial.print(".");
    i = i + 3;
    i % 2 == 0 ? digitalWrite(16, LOW):digitalWrite(16, HIGH);
    oled.drawUTF8(i, 20, " . ");
    oled.sendBuffer();
    if (i>45){
        oled.clearBuffer();
        oled.drawUTF8(0, 20, "连接中");
        i = 35;
      }
    }
  Serial.println("WiFi is connected");
  Serial.println(WiFi.localIP().toString());
  ip = WiFi.localIP().toString();
  digitalWrite(16, LOW);
  oled.clearBuffer();
  oled.drawUTF8(0, 20, WiFi.localIP().toString().c_str());
  oled.sendBuffer();
  delay(3000);
  oled.clearBuffer();
}
void NET_time_show(){
  time_t now = time(nullptr);
  struct tm * timeinfo = localtime(&now);
  char buffer[80];
  strftime(buffer, 80, "%H:%M:%S", timeinfo);
  oled.drawUTF8(0, 60, buffer);
  delay(1000);
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
  STA_set();
  HTTP_weather();
  configTime(8 * 3600, 0, "pool.ntp.org");
}

void loop() {
  // put your main code here, to run repeatedly:
  oled.clearBuffer();//清空缓冲区
  oled.drawUTF8(0, 20, ip.c_str());
  oled.drawUTF8(0, 40, weather_info.c_str());
  NET_time_show();
  oled.sendBuffer();//显示缓冲区内容
}
