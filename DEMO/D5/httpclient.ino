#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

HTTPClient http;
WiFiClient tcp;

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
  
  http.begin(tcp, "http://www.nmc.cn/f/rest/weather/57171");
  if(http.GET() == 200)
  {
    Serial.println(http.getString());
    //https://arduinojson.org/v6/assistant/
    const size_t capacity = JSON_ARRAY_SIZE(1) + JSON_ARRAY_SIZE(2) + 8*JSON_OBJECT_SIZE(2) + 4*JSON_OBJECT_SIZE(3) + 3*JSON_OBJECT_SIZE(4) + JSON_OBJECT_SIZE(5) + 530;
    DynamicJsonDocument doc(capacity);
    deserializeJson(doc, http.getString());
    const char* info = doc[0]["detail"][0]["day"]["weather"]["info"];
    const char* temp = doc[0]["detail"][0]["day"]["weather"]["temperature"];
    String line = String("今天白天") + info + "，气温" + temp + "℃";
    Serial.println(line);
  }
  http.end();
}

void loop() {
  // put your main code here, to run repeatedly:

}
