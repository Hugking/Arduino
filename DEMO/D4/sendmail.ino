#include <ESP8266WiFi.h>

void sendmail()
{
  WiFiClient mailclient;
  mailclient.connect("smtp.qq.com", 25);
  Serial.println(mailclient.readStringUntil('\n'));
  mailclient.println("ehlo localhost");
  Serial.println(mailclient.readString());
  mailclient.println("auth login");
  Serial.println(mailclient.readStringUntil('\n'));
  mailclient.println("QQ号base64编码");
  Serial.println(mailclient.readStringUntil('\n'));
  mailclient.println("授权码base64编码");
  Serial.println(mailclient.readStringUntil('\n'));
  mailclient.println("mail from:<xxx@qq.com>");
  Serial.println(mailclient.readStringUntil('\n'));
  mailclient.println("rcpt to:<xxx@189.cn>");
  Serial.println(mailclient.readStringUntil('\n'));
  mailclient.println("data");
  Serial.println(mailclient.readStringUntil('\n'));
  mailclient.println("from:<xxx@qq.com>");
  mailclient.println("to:<xxx@189.cn>");
  mailclient.println("subject:Hello from NodeMCU");
  mailclient.println();
  mailclient.println("hello, i am NodeMCU!");
  mailclient.println(".");
  Serial.println(mailclient.readStringUntil('\n'));
  mailclient.println("quit");
  Serial.println(mailclient.readStringUntil('\n'));
}

void setup() {
  Serial.begin(115200);
  Serial.println();
  WiFi.mode(WIFI_STA);
  WiFi.begin("Taro iPhone", "d7pe1uay97mfu");
  while (!WiFi.isConnected())
  {
    delay(100);
  }
  Serial.println("WiFi is connected");
  sendmail();
}

void loop() {
  // put your main code here, to run repeatedly:

}
