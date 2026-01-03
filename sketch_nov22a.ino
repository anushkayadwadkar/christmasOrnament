#include <Adafruit_NeoPixel.h>
#include <avr/sleep.h>
#include <avr/wdt.h>

#define LED_PIN 10
#define LDR_POWER_PIN 8    // Digital pin to power the sensor
#define LED_COUNT 4
#define PHOTO_PIN A0

#define DELAY 67
#define THRESHOLD_ON 3.1
#define THRESHOLD_OFF 3.4


Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
int v2=0;
bool ledState=false;


// --- WATCHDOG INTERRUPT ---
// This runs when the timer wakes the Uno up
ISR(WDT_vect) {
  // Do nothing, just waking up
}


void setup() {

  // put your setup code here, to run once:
  strip.begin();
  strip.show();
  strip.setBrightness(15);
  pinMode(PHOTO_PIN, INPUT);
  pinMode(LDR_POWER_PIN, OUTPUT);
  Serial.begin(9600);

  setupWatchdog();
}

void loop() {
  // put your main code here, to run repeatedly:

  // first check if we should proceed with turning on the circuit. 
  // if no, then skip remanining
  // if yes, then run the for loop

  // turn on the sensor
  digitalWrite(LDR_POWER_PIN, HIGH);
  delay(2);
  v2 = analogRead(PHOTO_PIN);
  digitalWrite(LDR_POWER_PIN, LOW);

  float v2_v = 0;
  v2_v = (5./1023.)*v2;
  
  Serial.println(v2_v);
  

  if (v2_v < THRESHOLD_ON) {
    turnOnLED();
  } else if (ledState == true) {
    if (v2_v > THRESHOLD_OFF) {
      // Now it is too bright. Don't turn on LEDs. Go to sleep.
      Serial.println("Status: DAY. Turning off Pixels. Going to Sleep...");
      turnOffLED();
      delay(100);
      enterSleep();
      Serial.println("Woke up!");
    } else {
      turnOnLED();
    }
  } else {
    Serial.println("Status: DAY. Turning off Pixels. Going to Sleep...");
      turnOffLED();
      delay(100);
      enterSleep();
      Serial.println("Woke up!");
  }
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


// --- HELPER: Deep Sleep Setup ---
void enterSleep() {
  ADCSRA &= ~(1<<ADEN); // Turn off ADC
  
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  
  // Note: On the Uno, this won't save much power because the USB chip
  // and power regulator stay on, but the Logic is identical to ATtiny.
  sleep_cpu(); 
  
  sleep_disable();
  ADCSRA |= (1<<ADEN); // Turn ADC back on
}

void setupWatchdog() {
  // UNO SPECIFIC REGISTER (WDTCSR instead of WDTCR)
  MCUSR &= ~(1<<WDRF);
  WDTCSR |= (1<<WDCE) | (1<<WDE);
  
  // Set to ~4 seconds
  // On Uno/ATmega328p: WDIE=1, WDP3=1, WDP0=0 => 1001 binary
  WDTCSR = (1<<WDIE) | (1<<WDP3); 
}

