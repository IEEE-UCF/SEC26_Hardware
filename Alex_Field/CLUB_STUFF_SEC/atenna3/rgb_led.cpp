#include "rgbled.h"
#include <Arduino.h>

rgbled::rgbled(int red, int green, int blue) {
  pinred = red;
  pingreen = green;
  pinblue = blue;
//  randomNumber =rand()%4;
  pinMode(pinred, OUTPUT);
  pinMode(pingreen, OUTPUT);
  pinMode(pinblue, OUTPUT);
  analogWrite(pinred, 0);
  analogWrite(pingreen, 0);
  analogWrite(pinblue, 0);
}
void rgbled::setColor(int pick) {
//  srand(time(NULL));
  switch (pick) {
  case 0:
    analogWrite(pinred, 255);
    break;
  case 1:
    analogWrite(pingreen, 255);
    break;
  case 2:
    analogWrite(pinblue, 255);
    break;
  case 3:
    analogWrite(pingreen, 255);
    analogWrite(pinred, 255);
    break;
  default:

    break;
  }
}
void rgbled::reset() {
  analogWrite(pinred, 0);
  analogWrite(pingreen, 0);
  analogWrite(pinblue, 0);
}
