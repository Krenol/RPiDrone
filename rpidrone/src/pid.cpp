#include "pid.hpp"
#include "easylogging++.h"

#define CONTROL_LOG(LEVEL) CLOG(LEVEL, "controls") //define controls log

namespace drone
{
    template <typename T>
    long PID<T>::getDt()
    {
        long dt;
        auto now = std::chrono::steady_clock::now();
        dt = std::chrono::duration_cast<std::chrono::milliseconds>(now - last_call_).count();
        last_call_ = now;
        return dt;
    }

    template <typename T>
    PID<T>::PID(T kp, T kd, T ki) : ki_{ki}, kd_{kd}, kp_{kp}, kaw_{0}, min_{0}, max_{0}, is_bound_{false}
    {
        CONTROL_LOG(INFO) << "Initliazied PID with kp=" << kp_ << " kd=" << kd_ << "ki=" << ki_ << " kaw=" << kaw_;
    }

    template <typename T>
    PID<T>::PID(T kp, T kd, T ki, T kaw, T min, T max) : ki_{ki}, kd_{kd}, kp_{kp}, kaw_{kaw}, min_{min}, max_{max}, is_bound_{true}
    {
        CONTROL_LOG(INFO) << "Initliazied PID with kp=" << kp_ << " kd=" << kd_ << "ki=" << ki_ << " kaw=" << kaw_ << " min=" << min_ << " max=" << max_;
    }

    template <typename T>
    T PID<T>::control(T is, T desired)
    {
        T out = 0, derivative = 0, err = is - desired;
        std::lock_guard<std::mutex> guard(mtx_);
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
            out = BOUND<T>(out, min_, max_);
            integral_ = BOUND<T>(integral_, -kaw_, kaw_);
        }
        first_call_ = false;
        return out;
    }
}