void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(D1, INPUT_PULLUP);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(digitalRead(D1));
  delay(100);
}
