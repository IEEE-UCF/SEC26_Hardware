/*
*/
#include <Arduino.h>
#include "forcesensor.h"
forcesensor::forcesensor(int pin)
{
    forcePin = pin;
    analogReading= 0;
    timetracker =0;
    task = NOTCOMPLETE;
}
void forcesensor::update()
{
    unsigned long currentmiliseconds = millis();
    unsigned timeInterval = currentmiliseconds-timetracker;
    if(timeInterval>=500)
    {
    timetracker = currentmiliseconds;
    analogReading = analogRead(forcePin);
    Serial.println("Force sensing....");
    if(analogReading<=30){
//        Serial.println("Duck is OFF!!");
        task=COMPLETE;
    }else{
//        Serial.println("Duck is on!");
    }
    }
}
int forcesensor::getStatus()
{
    if(task==COMPLETE){
        return 1;

    }
    else{
        return 0;
    }
}