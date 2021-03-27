#include "PID.h"
#include <Arduino.h>
#include "misc.h"


long PID::getDt()
{

    long dt;
    long now = millis();
    dt = now - last_call_;
    last_call_ = now;
    return dt;
}

PID::PID(float kp, float kd, float ki) : ki_{ki}, kd_{kd}, kp_{kp}, kaw_{0}, min_{0}, max_{0}, is_bound_{false}
{
}

PID::PID(float kp, float kd, float ki, float kaw, float min, float max) : ki_{ki}, kd_{kd}, kp_{kp}, kaw_{kaw}, min_{min}, max_{max}, is_bound_{true}
{
}

float PID::control(float is, float desired)
{
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
        out = BOUND(out, min_, max_);
        integral_ = BOUND(integral_, -kaw_, kaw_);
    }
    first_call_ = false;
    return out;
}