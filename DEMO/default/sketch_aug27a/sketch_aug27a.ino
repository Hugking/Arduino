void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(D1, INPUT_PULLUP);
  pinMode(16, OUTPUT);
//  digitalWrite(16, LOW);
  pinMode(A0, INPUT);
//  analogWrite(16, 1023);
}

void loop() {
  // put your main code here, to run repeatedly:
    for (int i=0;i<1024;i++) {
          analogWrite(16, 1023-i);
          delay(2);
      }
     for (int i=0;i<1024;i++) {
          analogWrite(16, i);
          delay(2);
      }
//  Serial.println(analogRead(A0));
//  if (Serial.available()){
////    String input = Serial.readString();
//      String input = Serial.readStringUntil('\n');
//    if (input == "on") {
//      digitalWrite(16, LOW);
//      }
//     if (input == "off") {
//        digitalWrite(16, HIGH);
//      }
////    input.toUpperCase();
////    Serial.println(input);
//    }
//    Serial.println(digitalRead(D1));
//    if (digitalRead(D1) == 0){
//      digitalWrite(16, HIGH);
//      }
//    else {
//      digitalWrite(16, LOW);
//      }
    delay(100);
}
