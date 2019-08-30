#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <FS.h>

ESP8266WebServer server;

void handle_root()
{
  File f = SPIFFS.open("/index.html", "r");
  server.streamFile(f, "text/html");
  f.close();
}

void handle_on()
{
  String html = "<html><body><p>LED On</p></body></html>";
  server.send(200, "text/html", html.c_str());
  digitalWrite(16, LOW);
}

void handle_off()
{
  String html = "<html><body><p>LED Off</p></body></html>";
  server.send(200, "text/html", html.c_str());
  digitalWrite(16, HIGH);
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
void setup() {
  Serial.begin(115200);
//  Serial.println();

  pinMode(16, OUTPUT);
  digitalWrite(16, HIGH);

  SPIFFS.begin();

  STA_set();
  server.on("/", handle_root);
  server.on("/on", handle_on);
  server.on("/off", handle_off);
  server.begin();
}

void loop() {
  server.handleClient();
}
