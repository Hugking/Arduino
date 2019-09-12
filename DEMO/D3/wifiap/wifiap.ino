#include <ESP8266WiFi.h>

WiFiServer tcpserver(666);

void setup() {
  Serial.begin(115200);
  Serial.println();

  pinMode(16, OUTPUT);
  digitalWrite(16, HIGH);
  
  WiFi.mode(WIFI_AP);
  WiFi.softAP("😁😁😁", "liuyu1234");
  Serial.println(WiFi.softAPIP().toString());

  tcpserver.begin();
}

void loop() {
  WiFiClient tcpclient = tcpserver.available();
  while (tcpclient.connected())
  {
    if (tcpclient.available())
    {
      String cmd = tcpclient.readStringUntil('\n');
//      Serial.println("'" + cmd + "'");
//      Serial.println(cmd.length());
//      for(int i = 0; i < cmd.length(); i++)
//      {
//        Serial.println(cmd[i], HEX);
//      }

      cmd.trim();

      if (cmd == "on")
      {//led on
        digitalWrite(16, LOW);
      }
      if (cmd == "off")
      {//led off
        digitalWrite(16, HIGH);
      }
    }
  }
}
