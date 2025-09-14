// Code used for Antenna 4 of the SoutheastCon Hardware Competition 2026

#include <Keypad.h>

int red = 9;
int green = 10;
int blue = 11;
int led_pin = 13;

int redBrightness = 0;
int greenBrightness = 0;
int blueBrightness = 0;
int randomColor;

const int ROW_NUM = 4; //four rows
const int COLUMN_NUM = 3; //three columns

char keys[ROW_NUM][COLUMN_NUM] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};

byte pin_rows[ROW_NUM] = {5, 6, 7, 8}; //connect to the row pinouts of the keypad
byte pin_column[COLUMN_NUM] = {2, 3, 4}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad(makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM );

const String password = "73738"; // change your password here
String input_password;

void setup(){
  Serial.begin(9600);
  input_password.reserve(32); // maximum input characters is 33, change if needed
  randomSeed(analogRead(A0));
  randomColor = random(5);
  Serial.println(randomColor);

  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(blue, OUTPUT);
  pinMode(led_pin, OUTPUT);

}

void loop(){
  char key = keypad.getKey();

  if (key){
    Serial.println(key);

    if(key == '*') {
      input_password = ""; // clear input password
    } 
    else if(key == '#') {
      if(password == input_password) {
        Serial.println("password is correct");
        
        Serial.print("LED On");
        digitalWrite(led_pin, HIGH);

        if(randomColor == 1)
        {
          redBrightness = 150;
          blueBrightness = 0;
          greenBrightness = 0;
        }
        else if(randomColor == 2)
        {
          redBrightness = 0;
          blueBrightness = 150;
          greenBrightness = 0;
        }
        else if(randomColor == 3)
        {
          redBrightness = 0;
          blueBrightness = 0;
          greenBrightness = 100;
        }
        else
        {
          redBrightness = 200;
          blueBrightness = 150;
          greenBrightness = 0; 
        }

        analogWrite(red, redBrightness);
        analogWrite(green, greenBrightness);
        analogWrite(blue, blueBrightness);
        digitalWrite(led_pin, HIGH);
        delay(3000);
        input_password = ""; // clear input password
        } 
        else {
          Serial.println("password is incorrect, try again");
          input_password = ""; // clear input password
        }
    } 
    else {
      input_password += key; // append new character to input password string
    }
  }
}
