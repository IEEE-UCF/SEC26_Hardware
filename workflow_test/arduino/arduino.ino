#include <Arduino.h>

int kLed = 1;
void setup() {
  pinMode(kLed, OUTPUT);
}

void loop() {
  delay(500);
  digitalWrite(kLed, HIGH);
  delay(500);
  digitalWrite(kLed, LOW);
}