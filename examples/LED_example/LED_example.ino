#include <tinyNeoPixel_Static.h>

// The four onboard SK6812 LEDs form one chain on PA3 (Arduino pin 4).
constexpr uint8_t LED_PIN = PIN_PA3;
constexpr uint8_t NUM_LEDS = 4;

// Optional button: connect a normally-open pushbutton between exposed pins
// PA1 (Arduino pin 2) and PA6 (Arduino pin 0). PA6 acts as ground and PA1
// uses its internal pull-up, so no external resistor is required.
constexpr uint8_t BUTTON_PIN = PIN_PA1;
constexpr uint8_t BUTTON_GND_PIN = PIN_PA6;
constexpr unsigned long DEBOUNCE_MS = 30;

constexpr unsigned long PHASE_MS = 500;
constexpr uint16_t HUE_STEP = 32;
constexpr unsigned long ROTATION_INTERVAL_MS = 15;
constexpr unsigned long HUE_SWEEP_INTERVAL_MS = 4;

// tinyNeoPixel_Static uses this fixed buffer instead of dynamic allocation.
byte pixelData[NUM_LEDS * 3];
tinyNeoPixel leds(NUM_LEDS, LED_PIN, NEO_GRB, pixelData);

enum Phase : uint8_t {
  PHASE_RED,
  PHASE_GREEN,
  PHASE_BLUE,
  PHASE_WHITE,
  PHASE_SWEEP,
  PHASE_CHASE,
  PHASE_COUNT
};

uint8_t phase = PHASE_RED;
uint16_t hue = 0;
uint8_t chaseIndex = 0;

unsigned long lastRotation = 0;
unsigned long lastPhaseChange = 0;
unsigned long lastHueStep = 0;

bool lastButtonReading = HIGH;
bool buttonState = HIGH;
unsigned long lastDebounceTime = 0;

void setAll(byte red, byte green, byte blue) {
  for (uint8_t i = 0; i < NUM_LEDS; i++) {
    leds.setPixelColor(i, red, green, blue);
  }
  leds.show();
}

void applyPhase() {
  switch (phase) {
    case PHASE_RED:
      setAll(255, 0, 0);
      break;
    case PHASE_GREEN:
      setAll(0, 255, 0);
      break;
    case PHASE_BLUE:
      setAll(0, 0, 255);
      break;
    case PHASE_WHITE:
      setAll(255, 255, 255);
      break;
    case PHASE_SWEEP:
      for (uint8_t i = 0; i < NUM_LEDS; i++) {
        leds.setPixelColor(i, leds.ColorHSV(hue));
      }
      leds.show();
      break;
    case PHASE_CHASE:
      for (uint8_t i = 0; i < NUM_LEDS; i++) {
        leds.setPixelColor(i, i == chaseIndex ? 255 : 0,
                           i == chaseIndex ? 255 : 0,
                           i == chaseIndex ? 255 : 0);
      }
      leds.show();
      break;
    default:
      break;
  }
}

void nextPhase() {
  phase = (phase + 1) % PHASE_COUNT;
  hue = 0;
  chaseIndex = 0;
  lastPhaseChange = millis();
  applyPhase();
}

void checkButton() {
  const bool reading = digitalRead(BUTTON_PIN);

  if (reading != lastButtonReading) {
    lastDebounceTime = millis();
  }

  if (millis() - lastDebounceTime > DEBOUNCE_MS && reading != buttonState) {
    buttonState = reading;
    if (buttonState == LOW) {
      nextPhase();
    }
  }

  lastButtonReading = reading;
}

void setup() {
  pinMode(LED_PIN, OUTPUT);

  pinMode(BUTTON_GND_PIN, OUTPUT);
  digitalWrite(BUTTON_GND_PIN, LOW);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  // Bright enough to see clearly without starting at maximum LED current.
  leds.setBrightness(20);

  lastPhaseChange = millis();
  lastHueStep = millis();
  lastRotation = millis();
  applyPhase();
}

void loop() {
  const unsigned long now = millis();

  checkButton();

  switch (phase) {
    case PHASE_RED:
    case PHASE_GREEN:
    case PHASE_BLUE:
    case PHASE_WHITE:
      if (now - lastPhaseChange >= PHASE_MS) {
        nextPhase();
      }
      break;

    case PHASE_SWEEP:
      if (now - lastHueStep >= HUE_SWEEP_INTERVAL_MS) {
        hue += HUE_STEP;
        applyPhase();
        lastHueStep = now;
      }
      break;

    case PHASE_CHASE:
      if (now - lastRotation >= ROTATION_INTERVAL_MS) {
        chaseIndex = (chaseIndex + 1) % NUM_LEDS;
        applyPhase();
        lastRotation = now;
      }
      break;

    default:
      break;
  }
}
