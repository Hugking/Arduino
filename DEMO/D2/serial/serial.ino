void setup() {
  Serial.begin(115200);
  Serial.println();
  pinMode(16, OUTPUT);
  digitalWrite(16, HIGH);
}

void loop() {
  if (Serial.available())
  {
    String input = Serial.readStringUntil('\n');
    if (input == "on")
    {
      digitalWrite(16, LOW);
    }
    if (input == "off")
    {
      digitalWrite(16, HIGH);
    }
  }
}
