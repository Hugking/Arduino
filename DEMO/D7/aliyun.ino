#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <SHA256.h>
#include <ArduinoJson.h>

const char* server = "a1zNmRN1Egn.iot-as-mqtt.cn-shanghai.aliyuncs.com";
const char* cid = "5ea3e6|securemode=3,signmethod=hmacsha256,timestamp=123|";
const char* username = "5ea3e6&a1zNmRN1Egn";
const char* hmacpwd = "clientId5ea3e6deviceName5ea3e6productKeya1zNmRN1Egntimestamp123";
const char* deviceSecret = "Xp5a4pZDFnHLBsWYkYlMTTfEro9MJPcA";

//topic
const char* topic_prop_set = "/sys/a1zNmRN1Egn/5ea3e6/thing/service/property/set";
const char* topic_prop_post = "/sys/a1zNmRN1Egn/5ea3e6/thing/event/property/post";

WiFiClient tcp;
PubSubClient mqttclient(tcp);

String hmacsha256(const String &signcontent, const String &ds)
{
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

int alarm_th = 1000;

void mqtt_callback(const char* topic, byte* payload, unsigned int length)
{
  if (!strcmp(topic, topic_prop_set))
  {
    //    payload[length] = 0;
    //    Serial.println((char*)payload);
    const size_t capacity = JSON_OBJECT_SIZE(1) + JSON_OBJECT_SIZE(4) + 90;
    DynamicJsonDocument doc(capacity);
    deserializeJson(doc, payload, length);
    alarm_th = doc["params"]["alarm_th"];
  }
}

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
  mqttclient.setServer(server, 1883);
  mqttclient.setCallback(mqtt_callback);
  if (mqttclient.connect(cid, username, hmacsha256(hmacpwd, deviceSecret).c_str()))
  {
    Serial.println("aliyun connected");
  }
  else
  {
    Serial.println("aliyun connect failed");
  }
  mqttclient.subscribe(topic_prop_set);
  configTime(3600*8, 0, "pool.ntp.org");
}

int msgid = 1;

void loop() {
  mqttclient.loop();
  
  const size_t capacity = JSON_OBJECT_SIZE(1) + JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(4);
  DynamicJsonDocument doc(capacity);

  doc["id"] = String(msgid);
  doc["version"] = "1.0";

  JsonObject params = doc.createNestedObject("params");

  JsonObject params_gas_con = params.createNestedObject("gas_con");
  params_gas_con["value"] = analogRead(A0);
  params_gas_con["time"] = time(nullptr);
  doc["method"] = "thing.event.property.post";

  String msg;
  serializeJson(doc, msg);
  Serial.println(msg);
  mqttclient.publish(topic_prop_post, msg.c_str());
  msgid++;
  delay(1000);
}
