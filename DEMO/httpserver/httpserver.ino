#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <FS.h>

ESP8266WebServer server;

void handle_root()
{
  //String html = "<html><body><p>Hello World</p></body></html>";
  //server.send(200, "text/html", html.c_str());
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

void setup() {
  Serial.begin(115200);
//  Serial.println();

  pinMode(16, OUTPUT);
  digitalWrite(16, HIGH);

  SPIFFS.begin();

  WiFi.mode(WIFI_STA);
  WiFi.begin("AK", "12345678");
  while(!WiFi.isConnected())
  {
    delay(1000);
    Serial.print(".");
  }
  Serial.println(WiFi.localIP().toString());
  server.on("/", handle_root);
  server.on("/on", handle_on);
  server.on("/off", handle_off);
  server.begin();
}

void loop() {
  server.handleClient();
}
