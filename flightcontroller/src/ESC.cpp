#include "ESC.h"
#include <Arduino.h>

ESC::ESC()
{
    initialized_ = false;
}

void ESC::init(uint8_t pin, uint16_t min_pulse_w, uint16_t max_pulse_w, uint16_t max_scale, bool calibrate) {
    this->max_scale_ = max_scale;
    esc_.attach(pin, min_pulse_w, max_pulse_w);
    if(calibrate) calibrateESC();
    initialized_ = true;
}

void ESC::calibrateESC() 
{
    setSpeed(0);
    setSpeed(max_scale_);
    delay(3000); //wait 3s
    setSpeed(0);
    delay(1000); //wait 1s
}

void ESC::setSpeed(uint16_t speed) 
{   
    if(!initialized_) return;
    Serial.println("set speed to " + speed);
    esc_.write(speed);
}

