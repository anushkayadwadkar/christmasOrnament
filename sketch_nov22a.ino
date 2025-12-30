#include <Adafruit_NeoPixel.h>

#define LED_PIN 0
#define LED_COUNT 4
#define DELAY 67


Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);


void setup() {

  // put your setup code here, to run once:
  strip.begin();
  strip.show();
  strip.setBrightness(15);

}

void loop() {
  // put your main code here, to run repeatedly:
  // int yellow = strip.Color(255,250,0);
  // blinky(yellow);

  // delay(200);
  // int cyan = strip.Color(0,255,255); //cyan
  // blinky(cyan);
  for (uint16_t startHue; startHue < 65535; startHue+=13070) {
    colorBlinky(startHue);
  }
}

void rainbow(int hue){
  strip.rainbow(hue);
  strip.show();
  delay(DELAY);
  strip.clear();
  strip.show();
  delay(DELAY);
  
}

void blinky(int color) {

 for (int i=0; i<4; i++) {
    strip.setPixelColor(i,color);
    strip.show();
    delay(DELAY);
    strip.clear();
  }
}


void colorBlinky(uint16_t startHue) {
  
  for (uint16_t i=0; i<4; i++) {
    uint32_t rgbColor = strip.ColorHSV(startHue+i*10000);
    
    strip.setPixelColor(i,rgbColor);
    strip.show();
    delay(DELAY);
    strip.clear();
    strip.show();
  }
}

