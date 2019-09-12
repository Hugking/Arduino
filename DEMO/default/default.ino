#include <FS.H>
void setup() {
  // put your setup code here, to run once:
  pinMode(16, OUTPUT);
  digitalWrite(16, LOW);
//  Serial.begin(115200);
//  SPIFFS.begin();
//  File f = SPIFFS.open('config.txt',w+);
//  if (!f) {
//      Serial.println("open file error");
//    }
  Serial.println("测试");
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(Do, HIGH);
  delay(500);
  digitalWrite(Do, LOW);
  delay(500);
}
