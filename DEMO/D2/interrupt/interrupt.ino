int t1;

//中断处理函数必须放到RAM中运行
void ICACHE_RAM_ATTR isr()
{
  //消抖处理
  if (millis() - t1 < 10) return;
  t1 = millis();
  Serial.println("button pressed");
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(D1, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(D1), isr, FALLING);
}

void loop() {
  // put your main code here, to run repeatedly:

}
