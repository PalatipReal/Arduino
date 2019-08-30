#include "LED.h"
#include "Arduino.h"

LED::LED(int pin){
    pinMode(pin,OUTPUT);
    _pin = pin;
}


void LED::On(int Timeinterval){
    digitalWrite(_pin,HIGH);
    delay(Timeinterval);
}

void LED::Off(int Timeinterval){
    digitalWrite(_pin,LOW);
    delay(Timeinterval);
}

void LED::Toggle(int Timeinterval){
    digitalWrite(_pin,LOW);
    delay(Timeinterval);
    digitalWrite(_pin,HIGH);
    delay(Timeinterval);
}
