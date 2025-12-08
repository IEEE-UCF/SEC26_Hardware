#include <Arduino.h>
#include "rgbled.h"
#include "DIYables_Keypad.h"
const int ROW_NUM =4;
const int COLUMN_NUM =3;

char keys[ROW_NUM][COLUMN_NUM] = 
{
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}

};
//in ascending order
byte row_pins[ROW_NUM]= 
//R1,R2,R3,R4
//7,2,3,5
{23,22,21,19}
;//look at a adafruit 3x4 phone-style matrix keypad(model 1824 or PID 1824)
byte column_pins[COLUMN_NUM] = 
//C1,C2,C3
//6,8,4
{33,25,18};
int led_pin=4;
int red = 5;
int green = 16;
int blue = 17;
DIYables_Keypad keypad = DIYables_Keypad(makeKeymap(keys),row_pins,column_pins,ROW_NUM,COLUMN_NUM);
rgbled rgb = rgbled(red,green,blue);
int randomNumber;
void setup()
{
Serial.begin(9600);
pinMode(led_pin,OUTPUT);
}
void loop()
{
keypad.update();
if(keypad.getStatus()==1)
{
digitalWrite(led_pin,HIGH);
rgb.setColor(3);
}
}