#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#include <U8g2lib.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

#include <SHA256.h>

#include <DHT.h>
DHT sensor(D7, DHT11);

U8G2_SSD1306_128X64_NONAME_F_SW_I2C oled(U8G2_R0, D4, D3);

String temp_humi_info;
String weather_info;
String ip;
int prev = 0;

WiFiClient tcp;
PubSubClient mqttclient(tcp);

const char* server = "a1pO4hAvOWd.iot-as-mqtt.cn-shanghai.aliyuncs.com";
const char* cid = "7f439e|securemode=3,signmethod=hmacsha256,timestamp=123|";
const char* username = "7f439e&a1pO4hAvOWd";
const char* hmacpwd = "clientId7f439edeviceName7f439eproductKeya1pO4hAvOWdtimestamp123";
const char* deviceSecret = "OfSqG1HdvsPdwAXCloaMIBKlYL94HKPo";

//topic
const char* topic_prop_set = "/sys/a1pO4hAvOWd/7f439e/thing/service/property/set";
const char* topic_prop_post = "/sys/a1pO4hAvOWd/7f439e/thing/event/property/post";

int alarm_th = 1000;
int msgid = 1;

#define TEMP_HUMI_TOPIC "pdsxy/3203/32/temp_humi"
#define LED_CTRL "pdsxy/3203/32/led"

String hmacsha256(const String &signcontent, const String &ds) {
  byte hash[32];
  const char* hex = "0123456789ABCDEF";
  SHA256 sha256;

  const char *key = ds.c_str();
  size_t keySize = ds.length();

  sha256.resetHMAC(key, keySize);
  sha256.update((const byte *)signcontent.c_str(), signcontent.length());
  sha256.finalizeHMAC(key, keySize, hash, sizeof(hash));

  String sign = "";
  for (byte i = 0; i < sizeof hash; ++i)
  {
    sign += hex[hash[i] >> 4];
    sign += hex[hash[i] & 0xf];
  }

  return sign;
}

void aliyun_callback(const char* topic, byte* payload, unsigned int length) {
  if (!strcmp(topic, topic_prop_set))
  {
    payload[length] = 0;
    Serial.println((char*)payload);
    const size_t capacity = JSON_OBJECT_SIZE(1) + JSON_OBJECT_SIZE(4) + 90;
    DynamicJsonDocument doc(capacity);
    deserializeJson(doc, payload, length);
    alarm_th = doc["params"]["alarm_th"];
    Serial.println(alarm_th);
  }
}
void aliyun_connect() {
  mqttclient.setServer(server, 1883);
  mqttclient.setCallback(aliyun_callback);
  if (mqttclient.connect(cid, username, hmacsha256(hmacpwd, deviceSecret).c_str()))
  {
    Serial.println("aliyun connected");
  }
  else
  {
    Serial.println("aliyun connect failed");
  }
  mqttclient.subscribe(topic_prop_set);
}
void aliyun_publish() {
  mqttclient.loop();
  if (millis() - prev > 60000) {
    prev = millis();

    const size_t capacity = JSON_OBJECT_SIZE(1) + JSON_OBJECT_SIZE(5);
    DynamicJsonDocument doc(capacity);

    doc["id"] = String(msgid);
    doc["method"] = "thing.event.property.post";
    JsonObject params = doc.createNestedObject("params");
    params["NO"] = digitalRead(16);
    doc["topic"] = "/sys/a1pO4hAvOWd/7f439e/thing/event/property/post";
    doc["version"] = "1.0";

    String msg;
    serializeJson(doc, msg);
    Serial.println(msg);
    mqttclient.publish(topic_prop_post, msg.c_str());
    msgid++;
    // 自动重连
  }
}

void MQTT_connect() {
  mqttclient.setServer("iot.tinytaro.cn", 1883);
  String clientID = String(ESP.getChipId(), HEX);
  Serial.print("client ID:" + clientID);
  mqttclient.setCallback(MQTT_callback);
  mqttclient.connect(clientID.c_str(), "hqyj", "opensesame");
  mqttclient.subscribe(LED_CTRL, 1);// 订阅主题
}
void MQTT_publish() {
  mqttclient.loop();
  if (millis() - prev > 10000) {
    prev = millis();
    Temp_humi();
    Serial.print(temp_humi_info);
    mqttclient.publish(TEMP_HUMI_TOPIC, temp_humi_info.c_str());
    // 自动重连
  }
}
void MQTT_callback(const char* topic, byte* payload, unsigned int length) {
  //  Serial.println(topic);
  //if (String(topic) == "pdsxy/3203/led")
  if (!strcmp(topic, LED_CTRL))
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

void Temp_humi() {
  float temp = sensor.readTemperature();
  float humi = sensor.readHumidity();
  temp_humi_info = String(temp) + "," + String(humi);
}

void HTTP_weather() {
  HTTPClient http;
  http.begin(tcp, "http://www.nmc.cn/f/rest/weather/57171");
  if (http.GET() == 200) {
    Serial.println(http.getString());
    //https://arduinojson.org/v6/assistant/
    const size_t capacity = JSON_ARRAY_SIZE(1) + JSON_ARRAY_SIZE(2) + 8 * JSON_OBJECT_SIZE(2) + 4 * JSON_OBJECT_SIZE(3) + 3 * JSON_OBJECT_SIZE(4) + JSON_OBJECT_SIZE(5) + 530;
    DynamicJsonDocument doc(capacity);
    deserializeJson(doc, http.getString());
    const char* info = doc[0]["detail"][0]["day"]["weather"]["info"];
    const char* temp = doc[0]["detail"][0]["day"]["weather"]["temperature"];
    const char* city = doc[0]["station"]["city"];
    weather_info = city + String("，") + info + "，" + temp + "℃";
    Serial.println(weather_info);
  }
  http.end();
}

void STA_set() {
  WiFi.mode(WIFI_STA);
  WiFi.begin("AK", "12345678");
  oled.clearBuffer();
  oled.drawUTF8(0, 20, "连接中");
  int i = 35 ;
  while (!WiFi.isConnected()) {
    delay(500);
    Serial.print(".");
    i = i + 3;
    i % 2 == 0 ? digitalWrite(16, LOW) : digitalWrite(16, HIGH);
    oled.drawUTF8(i, 20, " . ");
    oled.sendBuffer();
    if (i > 45) {
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

void NET_time_show() {
  time_t now = time(nullptr);
  struct tm * timeinfo = localtime(&now);
  char buffer[80];
  strftime(buffer, 80, "%H:%M:%S", timeinfo);
  oled.drawUTF8(0, 60, buffer);
  delay(1000);
}

void OLED_set() {
  oled.begin();
  oled.setFont(u8g2_font_wqy16_t_gb2312);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println();

  pinMode(16, OUTPUT);
  digitalWrite(16, LOW);

  OLED_set();
  STA_set();
  //  MQTT_connect();
  //  HTTP_weather();
  aliyun_connect();
  configTime(8 * 3600, 0, "pool.ntp.org");
}

void loop() {
  // put your main code here, to run repeatedly:
  oled.clearBuffer();//清空缓冲区
  oled.drawUTF8(0, 20, ip.c_str());
  //  oled.drawUTF8(0, 40, weather_info.c_str());
  oled.drawUTF8(0, 40, temp_humi_info.c_str());
  //  String lineA ="A";
  //  oled.drawUTF8(50, 50, lineA.c_str());
  NET_time_show();
  oled.sendBuffer();//显示缓冲区内容
  //  MQTT_publish();
  aliyun_publish();
}
