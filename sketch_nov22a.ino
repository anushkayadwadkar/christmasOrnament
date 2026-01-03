#include <Adafruit_NeoPixel.h>
#include <avr/sleep.h>
#include <avr/wdt.h>

#define LED_PIN 0
#define LDR_POWER_PIN 3    // Digital pin to power the sensor
#define LED_COUNT 4
#define PHOTO_PIN A1

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


  setupWatchdog();
}

void loop() {
  // put your main code here, to run repeatedly:

  // first check if we should proceed with turning on the circuit. 
  // if no, then skip remanining
  // if yes, then run the for loop

  // turn on the sensor

  float sensorReading = 0.0;

  sensorReading = readSensor();

  if (sensorReading < THRESHOLD_ON) {
    turnOnLED();
  } else if (ledState == true) {
    if (sensorReading > THRESHOLD_OFF) {
      // Now it is too bright. Don't turn on LEDs. Go to sleep.
      turnOffLED();
      enterSleep();
    } else {
      turnOnLED();
    }
  } else {
      turnOffLED();
      enterSleep();
    
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

float readSensor() {
  digitalWrite(LDR_POWER_PIN, HIGH);
  delay(2);
  v2 = analogRead(PHOTO_PIN);
  digitalWrite(LDR_POWER_PIN, LOW);

  float v2_v = 0;
  v2_v = (5./1023.)*v2;

  return v2_v;

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


// --- HELPER: Deep Sleep ---
void enterSleep() {
  ADCSRA &= ~(1<<ADEN);             // 1. Turn off ADC (saves huge power)
  set_sleep_mode(SLEEP_MODE_PWR_DOWN); // 2. Set deepest sleep mode
  sleep_enable();                   // 3. Enable sleep capability
  sleep_cpu();                      // 4. GO TO SLEEP NOW
  
  // -- CHIP SLEEPS HERE FOR ~4 SECONDS --
  
  sleep_disable();                  // 5. Woke up! Disable sleep
  ADCSRA |= (1<<ADEN);              // 6. Turn ADC back on
}

// --- HELPER: Watchdog Configuration (ATtiny85 Version) ---
void setupWatchdog() {
  MCUSR &= ~(1<<WDRF);              // Clear Reset Flag
  
  // Unlock the register (Safety Sequence)
  // Note: ATtiny85 uses "WDTCR", Arduino uses "WDTCSR"
  WDTCR |= (1<<WDCE) | (1<<WDE);

  // Set Interrupt Mode (WDIE) and ~4 Second Timeout (WDP3)
  WDTCR = (1<<WDIE) | (1<<WDP3) | (0<<WDP2) | (0<<WDP1) | (0<<WDP0);
}

