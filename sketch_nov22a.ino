#include <Adafruit_NeoPixel.h>

#define LED_PIN 10
#define LED_COUNT 4
#define PHOTO_PIN A0
#define DELAY 67
#define THRESHOLD_ON 3.1
#define THRESHOLD_OFF 3.4


Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
int v2=0;
bool ledState=false;


void setup() {

  // put your setup code here, to run once:
  strip.begin();
  strip.show();
  strip.setBrightness(15);
  pinMode(PHOTO_PIN, INPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:

  // first check if we should proceed with turning on the circuit. 
  // if no, then skip remanining
  // if yes, then run the for loop

  v2 = analogRead(PHOTO_PIN);

  float v2_v = 0;
  v2_v = (5./1023.)*v2;
  Serial.println(v2_v);
  

  if (v2_v < THRESHOLD_ON) {
    turnOnLED();
  } else if (ledState == true) {
    if (v2_v > THRESHOLD_OFF) {
      turnOffLED();
    } else {
      turnOnLED();
    }
  }

  delay(100);
 

}

void turnOffLED() {
  strip.clear();
  strip.show();
  ledState = false;
}

void turnOnLED() {
  for (uint32_t startHue=0; startHue < 65535; startHue+=13070) {
    colorBlinky(startHue);
  }
  ledState = true;
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

