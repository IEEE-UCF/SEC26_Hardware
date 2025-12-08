/*
Essentially, a pressure plate, sometimes called a force sensing resistor, force sensitive resistor,
or simply FSR. It is a type of a resistor that alters ITS OWN resistance when pressure
is applied to it
-effective at sensing physical PRESSURE or squeezing
-not effective at measuring the weight in lbs

This thing has two pins, becasue it works like a resistor, we do not need to differentiate betweent these pins
they're identical(I'm assuming the resistors)

All it does it changes its resistance based on how hard it is pressed.
The harder you press, the lower the resistance between its own two ends.
So in our case harder press is in the case of the duck on top of plate(low resistance)
take the duck off(high resistance)

Specifcally we nee ADC pins for these sensors
Connecting the force sensor to the analog input pin, we can read the analogRead() from it
it'll let us know the amount of pressure being applied to it
Honestly, any pin from the left side of the esp32 can give us a ADC
so, 
pin 36,37,38,39,32,33,34,35,4..etc
let's use a 10k ohm resistor

*/

#include <Arduino.h>
#ifndef FORCESENSOR_h   
#define FORCESENSOR_h
class forcesensor{
    public:
    forcesensor(int pin);
    void update();
    int getStatus();
    enum states{
        COMPLETE,
        NOTCOMPLETE
    }task;
    private:
    int analogReading{0};
    int forcePin{0};
    unsigned long timetracker{0};
};
#endif