#include <Arduino.h>
//#include "DIYables_Keypad.h"//the DIYable_Keypad Library
//#include "rgbled.h"
#include "forcesensor.h"
#define FORCE_SENSOR_PIN 27
forcesensor forcesensepad = forcesensor(FORCE_SENSOR_PIN);
void setup()
{
  Serial.begin(9600);
}

void loop()
{


  forcesensepad.update();
  if(forcesensepad.getStatus()==1)
  {
    Serial.println("HOORAY!");
  }
  // int analogReading = analogRead(FORCE_SENSOR_PIN);

  // Serial.print("Force sensor reading = ");
  // Serial.print(analogReading); // print the raw analog reading

  // if (analogReading < 10)       // from 0 to 9
  //   Serial.println(" -> no pressure");
  // else if (analogReading < 200) // from 10 to 199
  //   Serial.println(" -> light touch");
  // else if (analogReading < 500) // from 200 to 499
  //   Serial.println(" -> light squeeze");
  // else if (analogReading < 800) // from 500 to 799
  //   Serial.println(" -> medium squeeze");
  // else // from 800 to 1023
  //   Serial.println(" -> big squeeze");

  // delay(25);


//  rgb.setColor(0);
//rgb.reset();
//  digitalWrite(led_pin,HIGH);
//  delay(300);
  // keypad.update();
  // if(keypad.getStatus()==1)
  // {
  //   Serial.println("Task is completed!");
  //   digitalWrite(led_pin,HIGH);
  //   rgb.setColor(randomNumber);
   // rgb.setColor(3);
//}else{
   // Serial.println("Task is not finished!");
    // Serial.println("Task is not completed!");
    // digitalWrite(led_pin,LOW);
 // }

}
