#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

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
    String line = String("当前城市:") + city + "，今天白天" + info + "，气温" + temp + "℃";
    Serial.println(line);
 }
 http.end();
}
void STA_set(){
  WiFi.mode(WIFI_STA);
  WiFi.begin("AK", "12345678");
  while(!WiFi.isConnected()){
    delay(300);
    Serial.print(".");
    }
  Serial.println("WiFi is connected");
  Serial.println(WiFi.localIP().toString());
  digitalWrite(16, LOW);
  delay(3000);
}
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println();

  pinMode(16, OUTPUT);
  digitalWrite(16, LOW);

  STA_set();
  HTTP_weather();
}

void loop() {
  // put your main code here, to run repeatedly:

}
