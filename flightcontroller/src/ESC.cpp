#include "ESC.h"
#include <Arduino.h>

ESC::ESC()
{
    initialized_ = false;
}

void ESC::init(uint8_t pin, uint16_t min_pulse_w, uint16_t max_pulse_w, uint16_t max_scale, bool calibrate)
{
    if (initialized_)
        return;
    this->max_scale_ = max_scale;
    esc_.attach(pin, min_pulse_w, max_pulse_w);
    if (calibrate)
        calibrateESC();
    initialized_ = true;
    setSpeed(1);
    delay(200); 
    setSpeed(0);
}

void ESC::calibrateESC()
{
    if (initialized_)
        return;
    esc_.write(0);
    Serial.println("Disconnect the ESC from the battery and press enter");
    while (!Serial.available())
    {
    }
    esc_.write(max_scale_);
    Serial.println("Connect the ESC to the battery now");
    Serial.println("Wait for two beeps and a falling tone, then press enter");
    while (Serial.available() > 0)
    {
        char t = Serial.read();
    }
    while (!Serial.available())
    {
    }
    esc_.write(0);
    delay(6000); //wait 6s
    Serial.println("done calibrating");
}

void ESC::setSpeed(uint16_t speed)
{
    if (!initialized_)
        return;
    esc_.write(speed);
}
