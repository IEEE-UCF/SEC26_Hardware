#include "..\include\crank.h"
#include "..\include\rgbled.h"
#include <Arduino.h>

// pin configs
CrankConfig config = {13, 25, 33};
uint8_t r = 27;
uint8_t b = 26;
uint8_t g = 14;

int randomNum;

CrankP Crank = CrankP(config);
rgbled rgb = rgbled(r, b, g);

void setup() {
  Crank.begin();
  randomNum = random(0, 3);
}

void loop() {
  Crank.update();
  if (Crank.getStatus()) {
    rgb.set(randomNum);
  }
}