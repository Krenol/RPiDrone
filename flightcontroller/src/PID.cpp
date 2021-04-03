#include "PID.h"
#include <Arduino.h>
#include "misc.h"
#include <math.h>

long PID::getDt()
{
    long dt;
    long now = millis();
    dt = now - last_call_;
    last_call_ = now;
    return dt;
}

void PID::init() 
{
    first_call_ = true; 
    is_bound_ = false;
    integral_ = 0;
    last_error_ = 0;
    last_d_ = 0;
    last_call_ = millis();
}

PID::PID() 
{
    freqCut_ = 20;
    rc_ = 1 / (2 * M_PI * freqCut_);
    init_ = false;
}


float PID::control(float is, float desired)
{
    if(!init_) return 0;
    float out = 0, derivative = 0, err = is - desired;
    auto dt = getDt();

    out += kp_ * err;
    if (!first_call_)
    {
        derivative = (err - last_error_) / dt;
        derivative = last_d_ + ((dt / (rc_ + dt)) * (derivative - last_d_));
        integral_ = integral_ + err * dt;
    }
    out += kd_ * derivative + ki_ * integral_;
    last_error_ = err;
    last_d_ = derivative;
    if (is_bound_)
    {
        err = out;
        out = BOUND<float>(out, min_, max_);
        integral_ = BOUND<float>(integral_, -kaw_, kaw_);
    }
    first_call_ = false;
    return out;
}

void PID::set(float kp, float kd, float ki, float kaw, float min, float max) 
{
    kp_ = kp;
    ki_ = ki;
    kd_ = kd;
    kaw_ = kaw;
    min_ = min;
    max_ = max;
    is_bound_ = true;
    init();
    init_ = true;
}

void PID::set(float kp, float kd, float ki) 
{
    kp_ = kp;
    ki_ = ki;
    kd_ = kd;
    is_bound_ = false;
    init();
    init_ = true;
}