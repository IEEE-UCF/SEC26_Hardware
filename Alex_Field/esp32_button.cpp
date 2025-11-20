#include "..\include\button.h"
#include "..\include\rgbled.h"
#include <Arduino.h>

// pin configs
ButtonConfig config = {35, 5};
uint8_t r = 15;
uint8_t b = 4;
uint8_t g = 2;

int randomNum;

Button Button1 = Button(config);
rgbled rgb = rgbled(r, b, g);

void setup() {
  Button1.begin();
  randomNum = random(0, 3);
}

void loop() {
  Button1.update();
  if (Button1.getStatus()) {
    rgb.set(randomNum);
  }
}