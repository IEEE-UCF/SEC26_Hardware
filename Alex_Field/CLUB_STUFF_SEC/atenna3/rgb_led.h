/*
Need to make a class for rgbs
*/
#include <Arduino.h>
#ifndef RGBLED_H
#define RGBLED_H

class rgbled{
    public:
    rgbled(int red,int green,int blue);
    void setColor(int);
    void reset();
    //void off();
    private:
    int pinred{0};
    int pingreen{0};
    int pinblue{0};
    int randomNumber{0};
};
#endif