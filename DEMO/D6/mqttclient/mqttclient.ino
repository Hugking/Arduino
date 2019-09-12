#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>
#include <ArduinoJson.h>
#include <Adafruit_NeoPixel.h>

Adafruit_NeoPixel pixels(8, D2, NEO_GRB + NEO_KHZ800);
DHT sensor(D1, DHT11);
WiFiClient tcp;
PubSubClient mqttclient(tcp);

const char* TOPIC_LED = "pdsxy/3203/led";
const char* TOPIC_WILL = "pdsxy/3203/NodeMCU/status";
const char* TOPIC_HUE = "pdsxy/3203/huelight";

void mqtt_callback(const char* topic, byte* payload, unsigned int length)
{
  Serial.println(topic);
  //if (String(topic) == TOPIC_LED)
  if (!strcmp(topic, TOPIC_LED))
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
  if (!strcmp(topic, TOPIC_HUE))
  {
    const size_t capacity = JSON_OBJECT_SIZE(4) + 10;
    DynamicJsonDocument doc(capacity);
    deserializeJson(doc, payload, length);
    int h = doc["h"];
    float s = doc["s"];
    float v = doc["v"];
    int H = map(h*10, 0, 3600, 0, 65535);
    int S = map(s*1000, 0, 1000, 0, 255);
    int V = map(v*1000, 0, 1000, 0, 255);
    for (int i = 0; i < pixels.numPixels(); i++)
    {
      pixels.setPixelColor(i, pixels.ColorHSV(H, S, V));
    }
    pixels.show();
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println();
  sensor.begin();
  pinMode(16, OUTPUT);
  digitalWrite(16, HIGH);
  pixels.begin();

  WiFi.mode(WIFI_STA);
  WiFi.begin("Taro iPhone", "d7pe1uay97mfu");
  while (!WiFi.isConnected())
  {
    delay(100);
  }
  Serial.println("WiFi is connected");

  mqttclient.setServer("iot.tinytaro.cn", 1883);
  String clientID = String(ESP.getChipId(), HEX);
  Serial.println("Client ID: " + clientID);
  mqttclient.setCallback(mqtt_callback);
  mqttclient.connect(clientID.c_str(), "hqyj", "opensesame",
                     TOPIC_WILL, 0, 1, "offline");
  mqttclient.publish(TOPIC_WILL, "online", true); //birth message
  mqttclient.subscribe(TOPIC_LED, 1);
  mqttclient.subscribe(TOPIC_HUE, 1);
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
