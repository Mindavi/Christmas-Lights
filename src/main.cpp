// Copyright 2017 Rick van Schijndel

#include <Arduino.h>
#include "FastLED.h"
#include <ESP8266WiFi.h>
#include <Ticker.h>

#define NUM_LEDS 30
CRGB leds[NUM_LEDS];

int state = 0;
#define STATE_MAX 4

Ticker stateChanger;

void changeState() {
  state = (state + 1) % (STATE_MAX + 1);
}

void setup() {
  WiFi.mode(WIFI_OFF);
  pinMode(D2, OUTPUT);
  digitalWrite(D2, LOW);
  FastLED.addLeds<WS2812B, D2, GRB>(leds, NUM_LEDS);
  stateChanger.attach(30, changeState);
}

void makeColor(CRGB color, int wait = 50) {
  static uint16_t led_index = 0;
  led_index = (led_index + 1) % NUM_LEDS;
  leds[led_index] = color;
  FastLED.show();
  delay(wait);
}

// Create scrolling red and white candy cane stripes.
// Try adjusting the width in pixels for various results.
// Value "sets" should evenly divide into strand length
void candyCane(int sets, int width, int wait) {
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
  delay(wait);
}

CHSV randomColor() {
  CHSV hsv;
  hsv.hue = random(255);
  hsv.saturation = random(100, 255);
  hsv.value = 255;
  return hsv;
}

void randomColors(int wait = 500) {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = randomColor();
  }
  FastLED.show();
  delay(wait);
}

// https://github.com/FastLED/FastLED/wiki/Basic-usage#moving-an-led
void runningLed(CRGB color, int wait = 10) {
  static uint8_t dot = 0;
  dot = (dot + 1) % (NUM_LEDS);
  leds[dot] = color;
  FastLED.show();
  // clear this led for the next time around the loop
  leds[dot] = CRGB::Black;
  delay(wait);
}

void rainbow(int wait = 10) {
  static uint8_t hue = 0;
  FastLED.showColor(CHSV(hue++, 255, 255));
  delay(wait);
}

void loop() {
  switch (state) {
    case 0:
      makeColor(randomColor(), 50);
      break;
    case 1:
      candyCane(2, 4, 300);
      break;
    case 2:
      randomColors();
      break;
    case 3:
      rainbow();
      break;
    case 4:
      runningLed(randomColor(), 50);
      break;
    default:
      state = 0;
      break;
  }
}
