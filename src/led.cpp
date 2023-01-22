#include <Arduino.h>
#include "led.h"

Led::Led() {
    //init LEDs
    Leds[0] = 13;
    Leds[1] = 12;
    Leds[2] = 14;
    Leds[3] = 27;
    level = 0;
}

void Led::SetLeds() {
    if(level >= nLeds) {
        for(level; level > 0; level--) {
            analogWrite(Leds[level], 0);
        }
    }
    analogWrite(Leds[level], 2000);
    level++;
}