#ifndef RGBLED_CPP
#define RGBLED_CPP

#include "..\include\rgbled.h"

// Implements RGB
rgbled::rgbled(uint8_t pinr, uint8_t ping, uint8_t pinb)
    : _pinr(pinr), _ping(ping), _pinb(pinb) {
  pinMode(pinr, OUTPUT);
  pinMode(ping, OUTPUT);
  pinMode(pinb, OUTPUT);
  analogWrite(pinr, 0);
  analogWrite(ping, 0);
  analogWrite(pinb, 0);
};

// sets a specific color
void rgbled::set(int sel) {
  switch (sel) {
  case 1:
    analogWrite(_pinr, 150);
    break;
  case 2:
    analogWrite(_pinb, 150);
    break;
  case 3:
    analogWrite(_ping, 100);
    break;
  default:
    analogWrite(_pinb, 200);
    analogWrite(_pinr, 150);
    break;
  }
};

// resets led
void rgbled::reset() {
  analogWrite(_pinb, 0);
  analogWrite(_pinr, 0);
  analogWrite(_ping, 0);
}

#endif