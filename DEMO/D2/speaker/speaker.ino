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
  //  pinMode(D1, OUTPUT);
  //  analogWriteFreq(500);
}

void loop() {
  // put your main code here, to run repeatedly:
  //  digitalWrite(D1, HIGH);
  //  delayMicroseconds(500);  //延时500微秒，1秒 = 1000毫秒 = 1000 000微秒
  //  digitalWrite(D1, LOW);
  //  delayMicroseconds(500);
  //  analogWrite(D1, 512);
  for (int i = 0; i < ARRAY_SIZE(melody); i++)
  {
    tone(D1, melody[i]);
    delay(BEAT/noteDurations[i]);
    noTone(D1);
    delay(BEAT/noteDurations[i]);
  }
  delay(1000);
}
