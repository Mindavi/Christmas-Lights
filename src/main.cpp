// Copyright 2017 Rick van Schijndel

#include <Arduino.h>
#include "FastLED.h"
#include "Ticker.h"

#define NUM_LEDS 30
CRGB leds[NUM_LEDS];

#define STATE_MAX 4

Ticker stateChanger;
Ticker effectShower;

void changeState();

void setup() {
  pinMode(2, OUTPUT);
  digitalWrite(2, LOW);
  FastLED.addLeds<WS2812B, 2, GRB>(leds, NUM_LEDS);
  stateChanger.setInterval(30 * 1000);
  stateChanger.setCallback(changeState);
  stateChanger.start();
}

CHSV randomColor() {
  CHSV hsv;
  hsv.hue = random(255);
  hsv.saturation = random(100, 255);
  hsv.value = 255;
  return hsv;
}

void makeColor(CRGB color) {
  static uint16_t led_index = 0;
  led_index = (led_index + 1) % NUM_LEDS;
  leds[led_index] = color;
  FastLED.show();
}

void makeRandomColor() {
  makeColor(randomColor());
}

// Create scrolling red and white candy cane stripes.
// Try adjusting the width in pixels for various results.
// Value "sets" should evenly divide into strand length
void candyCane(int sets, int width) {
  int L;
  static uint16_t j = 0;
  j = (j + 1) % (sets * width);
  for (int i = 0; i < NUM_LEDS; i++) {
    L = NUM_LEDS - i - 1;
    if (((i + j) % (width * 2)) < width)
      leds[L] = CRGB::Red;
    else
      leds[L] = CRGB::White;
  }
  FastLED.show();
}

void candyCaneCallback() {
  candyCane(2, 4);
}

void randomColors() {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = randomColor();
  }
  FastLED.show();
}

// https://github.com/FastLED/FastLED/wiki/Basic-usage#moving-an-led
void runningLed(CRGB color) {
  static uint8_t dot = 0;
  dot = (dot + 1) % (NUM_LEDS);
  leds[dot] = color;
  FastLED.show();
  // clear this led for the next time around the loop
  leds[dot] = CRGB::Black;
}

void randomRunningLed() {
  runningLed(randomColor());
}

void rainbow() {
  static uint8_t hue = 0;
  FastLED.showColor(CHSV(hue++, 255, 255));
}

void changeState() {
  static int state = 0;
  state = (state + 1) % (STATE_MAX + 1);
  effectShower.resume();

  switch (state) {
    case 0:
      effectShower.setCallback(makeRandomColor);
      effectShower.setInterval(50);
      break;
    case 1:
      effectShower.setCallback(candyCaneCallback);
      effectShower.setInterval(300);
      break;
    case 2:
      effectShower.setCallback(randomColors);
      effectShower.setInterval(350);
      break;
    case 3:
      effectShower.setCallback(rainbow);
      effectShower.setInterval(7);
      break;
    case 4:
      effectShower.setCallback(randomRunningLed);
      effectShower.setInterval(25);
    default:
      state = 0;
      break;
  }
}

void loop() {
  stateChanger.update();
  effectShower.update();
}
