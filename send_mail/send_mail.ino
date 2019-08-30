#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

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
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println();

  pinMode(A0,OUTPUT); 
//  Send_mail();
}

void loop() {
  // put your main code here, to run repeatedly:
  Send_mail_once();
}
