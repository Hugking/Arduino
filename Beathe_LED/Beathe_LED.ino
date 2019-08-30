
void Breath_led(){
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
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println();

  pinMode(16, OUTPUT);
  digitalWrite(16, LOW);
  pinMode(A0,OUTPUT); 
}

void loop() {
  // put your main code here, to run repeatedly:
  Breath_led();
}
