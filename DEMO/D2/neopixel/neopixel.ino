#include <Adafruit_NeoPixel.h>

Adafruit_NeoPixel pixels(12, D1, NEO_GRB + NEO_KHZ800);

void setup() {
  pixels.begin();
}

unsigned short hue;

void loop() {
  for(int i = 0; i < pixels.numPixels(); i++)
  {
    pixels.setPixelColor(i, pixels.ColorHSV(hue,255,1));
  }
  pixels.show();
  hue += 10;
  delay(1);
}
