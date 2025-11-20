#ifndef RGBLED_H
#define RGBLED_H

#include <Arduino.h>

// defines RGB
class rgbled {
private:
  int _pinr;
  int _ping;
  int _pinb;

public:
  rgbled(uint8_t pinr, uint8_t ping, uint8_t pinb);
  void set(int sel);
  void reset();
};

#endif