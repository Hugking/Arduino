#include <U8g2lib.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <ESP8266WebServer.h>
#include <FS.h>

WiFiClient tcp;
WiFiServer tcpserver(666);
ESP8266WebServer server;
String weather_info;
String ip;
U8G2_SSD1306_128X64_NONAME_F_SW_I2C oled(U8G2_R0, D4, D3);
void OLED_set(){
  oled.begin();
  oled.setFont(u8g2_font_wqy16_t_gb2312);
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
void NET_time_show(){
  time_t now = time(nullptr);
  struct tm * timeinfo = localtime(&now);
  char buffer[80];
  strftime(buffer, 80, "%H:%M:%S", timeinfo);
  oled.drawUTF8(0, 60, buffer);
  delay(1000);
}
void Send_mail(){
  WiFiClient mailclient;
  mailclient.connect("smtp.qq.com", 25); 
  Serial.println(mailclient.readStringUntil('\n'));
  mailclient.println("ehlo Localhost");
  Serial.println(mailclient.readString());
  mailclient.println("auth login");
  Serial.println(mailclient.readStringUntil('\n'));
  mailclient.println("Mjk5Mzk4MDI5OA==");//QQ号base64编码
  Serial.println(mailclient.readStringUntil('\n'));
  mailclient.println("aWdxZW92ZXJ3YXhpZGVjZQ==");//授权码base64编码igqeoverwaxidece
  Serial.println(mailclient.readStringUntil('\n'));
  mailclient.println("mail from:<2993980298@qq.com>");
  Serial.println(mailclient.readStringUntil('\n'));
  mailclient.println("rcpt to:<2993980298@qq.com>");
  Serial.println(mailclient.readStringUntil('\n'));
  mailclient.println("data");
  Serial.println(mailclient.readStringUntil('\n'));
  mailclient.println("from:<2993980298@qq.com>");
  mailclient.println("to:<2993980298@qq.com>");
  mailclient.println("subject:Hello from NodeMCU");
  mailclient.println("");
  mailclient.println("hello, i am NodeMCU!");
  mailclient.println(".");
  Serial.println(mailclient.readStringUntil('\n'));
  mailclient.println("quit");
  Serial.println(mailclient.readStringUntil('\n'));
}
void Send_mail_once(){
  float buffers = analogRead(A0);
  Serial.println(buffers);
  int n;
  if(buffers>=95&&n==0)
  {
    Send_mail();
    n++;
  }
  if(buffers<95)
  {
    n=0;
  }
  delay(100);
};
void Breath_led(){
    for(int i = 0; i < 1024; i++) //由暗变亮
  {
    analogWrite(16, 1023 - i);
    delay(2);
  }
  
  for(int i = 0; i < 1024; i++) //由亮变暗
  {
    analogWrite(16, i);
    delay(2);
  }
}
void HTTP_weather(){
  HTTPClient http;
  http.begin(tcp, "http://www.nmc.cn/f/rest/weather/57171");
  if (http.GET() == 200){
    Serial.println(http.getString());
    //https://arduinojson.org/v6/assistant/
    const size_t capacity = JSON_ARRAY_SIZE(1) + JSON_ARRAY_SIZE(2) + 8*JSON_OBJECT_SIZE(2) + 4*JSON_OBJECT_SIZE(3) + 3*JSON_OBJECT_SIZE(4) + JSON_OBJECT_SIZE(5) + 530;
    DynamicJsonDocument doc(capacity);
    deserializeJson(doc, http.getString());
    const char* info = doc[0]["detail"][0]["night"]["weather"]["info"];
    const char* temp = doc[0]["detail"][0]["night"]["weather"]["temperature"];
    const char* city = doc[0]["station"]["city"];
    weather_info = city + String("，") + info + "，" + temp + "℃";
    Serial.println(weather_info);
 }
 http.end();
}
void WEB_server_set(){
  server.on("/", handel_root);
  server.on("/on", handel_on);
  server.on("/off", handel_off);
  server.on("/default", handel_default);
  server.begin();
}
void handel_root(){
  String html = "<html><body>hello world!</body></html>";
  server.send(200, "text/html", html.c_str());
}
void handel_default(){
  File f = SPIFFS.open("/index.html", "r");
  server.streamFile(f, "text/html");
  f.close();
}
void handel_on(){
  String html = "<html><body>led on !</body></html>";
  server.send(200, "text/html", html.c_str());
  digitalWrite(16, LOW);
}
void handel_off(){
  String html = "<html><body>led off !</body></html>";
  server.send(200, "text/html", html.c_str());
  digitalWrite(16, HIGH);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println();

  pinMode(16, OUTPUT);
  digitalWrite(16, LOW);
  pinMode(A0,OUTPUT); 
  
  SPIFFS.begin();
  OLED_set();
//  AP_set();
  STA_set();
//  Send_mail();
  HTTP_weather();
  WEB_server_set();
  configTime(8 * 3600, 0, "pool.ntp.org");
}

void loop() {
  // put your main code here, to run repeatedly:
  // TCP_print();
    server.handleClient();
    oled.clearBuffer();//清空缓冲区
    oled.drawUTF8(0, 20, ip.c_str());
    oled.drawUTF8(0, 40, weather_info.c_str());
    NET_time_show();
    oled.sendBuffer();//显示缓冲区内容
//     Breath_led();
}
