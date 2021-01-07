#include <mutex>
#include <chrono>
#include <atomic>
#include <misc.hpp>
#include "easylogging++.h"

#define CONTROL_LOG(LEVEL) CLOG(LEVEL, "controls") //define controls log

#ifndef DRONE_PID_H
#define DRONE_PID_H

namespace drone
{
    template <typename T = float>
    class PID
    {
    private:
        const int freqCut_{20};
        const T ki_, kd_, kp_, kaw_, min_, max_, rc_ = 1 / (2 * M_PI * freqCut_);
        T integral_{0}, last_error_{0}, last_d_{0};
        const bool is_bound_{false};
        std::chrono::steady_clock::time_point last_call_ = std::chrono::steady_clock::now();
        std::atomic_bool first_call_{true};
        std::mutex mtx_;

        long getDt() {
            long dt;
            auto now = std::chrono::steady_clock::now();
            dt = std::chrono::duration_cast<std::chrono::milliseconds>(now - last_call_).count();
            last_call_ = now;
            return dt;
        }

    public:
        /**
         * Constructor for PID
         * @param kp Value for proportional part
         * @param kd Value for differential part
         * @param ki Value for integral part
         * @param kaw Anti windup param for controller
         */
        PID(T kp = 1.0, T kd = 1.0, T ki = 1.0) : ki_{ki}, kd_{kd}, kp_{kp}, kaw_{0}, min_{0}, max_{0}, is_bound_{false}
        {
            CONTROL_LOG(INFO) << "Initliazied PID with kp=" << kp_ << " kd=" << kd_ << "ki=" << ki_ << " kaw=" << kaw_;
        }

        /**
         * Constructor for PID
         * @param kp Value for proportional part
         * @param kd Value for differential part
         * @param ki Value for integral part
         * @param kaw Anti windup param for controller
         * @param min Min value of the controller
         * @param max max value of the controller
         */
        PID(T kp, T kd, T ki, T kaw, T min, T max) : ki_{ki}, kd_{kd}, kp_{kp}, kaw_{kaw}, min_{min}, max_{max}, is_bound_{true}
        {
            CONTROL_LOG(INFO) << "Initliazied PID with kp=" << kp_ << " kd=" << kd_ << "ki=" << ki_ << " kaw=" << kaw_ << " min=" << min_ << " max=" << max_;
        }

        /**
         * Method to get control value 
         * @param is The is state of the system
         * @param desired The desired state
         */
        T control(T is, T desired) 
        {
            T out = 0, derivative = 0, err = is - desired;
            std::lock_guard<std::mutex> guard(mtx_);
            auto dt = getDt();
            
            out += kp_ * err;
            if(!first_call_) {
                derivative = (err - last_error_) / dt;
                derivative = last_d_ + ((dt / (rc_ + dt)) * (derivative - last_d_));
                integral_ = integral_ + err * dt;
            }
            out += kd_ * derivative + ki_ * integral_;
            last_error_ = err;
            last_d_ = derivative;
            if(is_bound_) {
                err = out;
                out = BOUND<T>(out, min_, max_);
                integral_ = BOUND<T>(integral_, -kaw_, kaw_);
            }
            first_call_ = false;
            return out;
        }
    };
} // namespace drone

#endif