// Code for Antenna #1 for the Southeast Con 2026 Hardware Competition
// The code requires the library ezButton 1.0.4 to be installed

#include <Arduino.h>
#include <ezButton.h>  // The library to use for SW pin

#define SW_PIN 4

int randomColor;
int counter = 0;

int red = 9;
int green = 10;
int blue = 11;

int redBrightness = 0;
int greenBrightness = 0;
int blueBrightness = 0;
int redLED = 5;
int yellowLED = 6;
int greenLED = 7;
int led_pin = 13;

ezButton button(SW_PIN);

void setup() {
  Serial.begin(9600);

  randomSeed(analogRead(A0));
  randomColor = random(1, 5);

  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(blue, OUTPUT);
  pinMode(redLED, OUTPUT);
  pinMode(yellowLED, OUTPUT);
  pinMode(greenLED, OUTPUT);
  pinMode(led_pin, OUTPUT);
}

void loop() {
  button.loop(); 

  if (button.isPressed()) {
    Serial.println("The button is pressed");
    counter++;

    if (counter == 1) { // First button press
      digitalWrite(redLED, HIGH);
    } 
    else if (counter == 2) { // Second button press
      digitalWrite(yellowLED, HIGH);
    }  
    else if (counter == 3) { // Third button press
      digitalWrite(greenLED, HIGH);
      if (randomColor == 1) {
        redBrightness = 150;
        blueBrightness = 0;
        greenBrightness = 0;
      } else if (randomColor == 2) {
        redBrightness = 0;
        blueBrightness = 150;
        greenBrightness = 0;
      } else if (randomColor == 3) {
        redBrightness = 0;
        blueBrightness = 0;
        greenBrightness = 100;
      } else {
        redBrightness = 200;
        blueBrightness = 150;
        greenBrightness = 0;
      }

      analogWrite(red, redBrightness);
      analogWrite(green, greenBrightness);
      analogWrite(blue, blueBrightness);
      digitalWrite(led_pin, HIGH);
      delay(3000);
    }
    delay(500);

  }
}