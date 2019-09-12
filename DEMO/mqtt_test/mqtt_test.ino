#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>
#include <ArduinoJson.h>

DHT sensor(D1, DHT11);
WiFiClient tcp;
PubSubClient mqttclient(tcp);

void mqtt_callback(const char* topic, byte* payload, unsigned int length)
{
  Serial.println(topic);
  //if (String(topic) == "pdsxy/3203/led")
  if (!strcmp(topic, "pdsxy/3203/led"))
  {
    //{"status":"on"}
    const size_t capacity = JSON_OBJECT_SIZE(1) + 20;
    DynamicJsonDocument doc(capacity);
    deserializeJson(doc, payload, length);
    const char* status = doc["status"];
    if (String(status) == "on")
    {
      digitalWrite(16, LOW);
    }
    if (String(status) == "off")
    {
      digitalWrite(16, HIGH);
    }
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println();
  sensor.begin();
  pinMode(16, OUTPUT);
  digitalWrite(16, HIGH);

  WiFi.mode(WIFI_STA);
  WiFi.begin("AK", "12345678");
  while (!WiFi.isConnected())
  {
    delay(100);
  }
  Serial.println("WiFi is connected");

  mqttclient.setServer("iot.tinytaro.cn", 1883);
  String clientID = String(ESP.getChipId(), HEX);
  Serial.println("Client ID: " + clientID);
  mqttclient.setCallback(mqtt_callback);
  mqttclient.connect(clientID.c_str(), "hqyj", "opensesame");
  mqttclient.subscribe("pdsxy/3203/led", 1);
}

int prev = 0;

void loop() {
  mqttclient.loop();
  if (millis() - prev > 60000)
  {
    prev = millis();
    float temp = sensor.readTemperature();
    float humi = sensor.readHumidity();
    String line = String(temp) + "," + String(humi);
    Serial.println(line);
    mqttclient.publish("pdsxy/3203/sensor", line.c_str());
  }
}
