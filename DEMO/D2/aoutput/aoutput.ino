void setup() {
  // put your setup code here, to run once:
  pinMode(16, OUTPUT);
  analogWrite(16, 1023);
}

void loop() {
  // put your main code here, to run repeatedly:
  for(int i = 0; i < 1024; i++) //由暗变亮
  {
    analogWrite(16, 1023 - i);
    delay(2);
  }
  
  for(int i = 0; i < 1024; i++) //由亮变暗
  {
    analogWrite(16, i);
    delay(2);
  }
}
