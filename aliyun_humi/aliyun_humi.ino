#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <U8g2lib.h>
#include <ArduinoJson.h>
#include <SHA256.h>

U8G2_SSD1306_128X64_NONAME_F_SW_I2C oled(U8G2_R0, D4, D3);
WiFiClient tcp;
PubSubClient mqttclient(tcp);

#include <DHT.h>
DHT sensor(D1, DHT11);

const char* server = "a1PsnbJWMlo.iot-as-mqtt.cn-shanghai.aliyuncs.com";
const char* cid = "afd6e|securemode=3,signmethod=hmacsha256,timestamp=123|";
const char* username = "afd6e&a1PsnbJWMlo";
const char* hmacpwd = "clientIdafd6edeviceNameafd6eproductKeya1PsnbJWMlotimestamp123";
const char* deviceSecret = "BFtH0jyPOQ5EWsYQJNQe7QoKTNcRloxg";

//topic
const char* topic_prop_set = "/sys/a1PsnbJWMlo/afd6e/thing/service/property/set";
const char* topic_prop_post = "/sys/a1PsnbJWMlo/afd6e/thing/event/property/post";

int alarm_th = 1000;
int msgid = 1;
String ip;
int prev = 0;
String temp_humi_info;
String message;

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
  }
}

void aliyun_connect() {
  mqttclient.setServer(server, 1883);
  String clientID = String(ESP.getChipId(), HEX);
  Serial.print("client ID:" + clientID);
  message = clientID;
  mqttclient.setCallback(aliyun_callback);
  if (mqttclient.connect(cid, username, hmacsha256(hmacpwd, deviceSecret).c_str()))
  {
    Serial.println("aliyun connected");
    message = "阿里云连接成功";
  }
  else
  {
    Serial.println("aliyun connect failed");
    message = "阿里云连接失败";
  }
  mqttclient.subscribe(topic_prop_set);
}

void aliyun_publish() {
  mqttclient.loop();
  __auto_publish();
}

void switch_publish() {
  const size_t capacity = JSON_OBJECT_SIZE(1) + JSON_OBJECT_SIZE(5);
  DynamicJsonDocument doc(capacity);

  doc["id"] = String(msgid);
  doc["method"] = "thing.event.property.post";
  JsonObject params = doc.createNestedObject("params");
  params["PowerSwitch"] = digitalRead(16);
  doc["topic"] = "/sys/a1pO4hAvOWd/7f439e/thing/event/property/post";
  doc["version"] = "1.0";

  String msg;
  serializeJson(doc, msg);
  Serial.println(msg);
  mqttclient.publish(topic_prop_post, msg.c_str());
  msgid++;
}

void temp_humi_pulish() {
  const size_t capacity = JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(5);
  DynamicJsonDocument doc(capacity);

  doc["id"] = String(msgid);
  doc["method"] = "thing.event.property.post";
  JsonObject params = doc.createNestedObject("params");
  float temp = sensor.readTemperature();
  float humi = sensor.readHumidity();
  message = String(temp) + "," + String(humi);
  params["mhumi"] = humi;
  params["mtemp"] = temp;
  doc["topic"] = "/sys/a1pO4hAvOWd/7f439e/thing/event/property/post";
  doc["version"] = "1.0";

  String msg;
  serializeJson(doc, msg);
  Serial.println(msg);
  mqttclient.publish(topic_prop_post, msg.c_str());
  msgid++;
}


void __auto_publish() {
  if (millis() - prev > 10000) {
    prev = millis();

    if (mqttclient.connect(cid, username, hmacsha256(hmacpwd, deviceSecret).c_str())) {
      //      Serial.println("在线");
      message = "设备在线";
    }
    else {
      //      Serial.println("离线");
      message = "设备离线";
      STA_set();
      aliyun_connect();
    }
    temp_humi_pulish();
  }
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
  aliyun_connect();
  configTime(8 * 3600, 0, "pool.ntp.org");

}

void loop() {
  // put your main code here, to run repeatedly:
  oled.clearBuffer();//清空缓冲区
  oled.drawUTF8(0, 20, ip.c_str());
  oled.drawUTF8(0, 40, message.c_str());
  NET_time_show();
  aliyun_publish();
  oled.sendBuffer();//显示缓冲区内容

}
