#include <ESP8266WiFi.h>
void STA_set(){
  WiFi.mode(WIFI_STA);
  WiFi.begin("AK", "12345678");
  while(!WiFi.isConnected()){
    delay(300);
    Serial.print(".");
    }
  Serial.println("WiFi is connected");
  Serial.println(WiFi.localIP().toString());
  delay(1000);
}

void AP_set(){
  WiFi.mode(WIFI_AP);
  WiFi.softAP("@", "12345678");
  Serial.println(WiFi.softAPIP().toString());
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println();
  //  AP_set();
  STA_set();
}

void loop() {
  // put your main code here, to run repeatedly:

}
