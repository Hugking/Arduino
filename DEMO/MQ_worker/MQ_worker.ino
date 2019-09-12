#include "pitches.h"

#define ARRAY_SIZE(a) (sizeof(a)/sizeof((a)[0]))

#define BEAT (70*1000/60)

int melody[] = {
  NOTE_C4, NOTE_C4, NOTE_G4, NOTE_G4, NOTE_A4, NOTE_A4, NOTE_G4,
  NOTE_F4, NOTE_F4, NOTE_E4, NOTE_E4, NOTE_D4, NOTE_D4, NOTE_C4,
  NOTE_G4, NOTE_G4, NOTE_F4, NOTE_F4, NOTE_E4, NOTE_E4, NOTE_D4,
  NOTE_G4, NOTE_G4, NOTE_F4, NOTE_F4, NOTE_E4, NOTE_E4, NOTE_D4,
  NOTE_C4, NOTE_C4, NOTE_G4, NOTE_G4, NOTE_A4, NOTE_A4, NOTE_G4,
  NOTE_F4, NOTE_F4, NOTE_E4, NOTE_E4, NOTE_D4, NOTE_D4, NOTE_C4,
};

//音符持续时间，4为4分音符，2为2分音符。
int noteDurations[] = {
  4, 4, 4, 4, 4, 4, 2,
  4, 4, 4, 4, 4, 4, 2,
  4, 4, 4, 4, 4, 4, 2,
  4, 4, 4, 4, 4, 4, 2,
  4, 4, 4, 4, 4, 4, 2,
  4, 4, 4, 4, 4, 4, 2,
};
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(A0, INPUT);
  pinMode(D2, INPUT_PULLUP);//蜂鸣器
  pinMode(D1, INPUT_PULLUP);//按键

}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available())
  {  
     Serial.println(analogRead(A0),digitalRead(D1));
     if (analogRead(A0) >= 95){
        if(digitalRead(D1) != 0 ){
            for (int i = 0; i < ARRAY_SIZE(melody); i++)
            {
              tone(D2, melody[i]);
              delay(BEAT/noteDurations[i]);
              noTone(D2);
              delay(BEAT/noteDurations[i]);
            }
            delay(1000);
          }
         else {
            noTone(D2);
          }
      }
  }
  delay(1000);
}
