#include <mutex>
#include <chrono>
#include <atomic>
#include <misc.hpp>

#ifndef DRONE_PID_H
#define DRONE_PID_H

namespace drone
{
    //https://stackoverflow.com/a/28876046
    template <typename T>
    struct is_chrono_duration
    {
        static constexpr bool value = false;
    };

    template <typename Rep, typename Period>
    struct is_chrono_duration<std::chrono::duration<Rep, Period>>
    {
        static constexpr bool value = true;
    };

    template <typename X = float, typename T = std::chrono::milliseconds>
    class PID
    {
    private:
        const X ki_, kd_, kp_, kaw_, min_, max_;
        X integral_, last_error_;
        const bool is_bound_{false};
        std::chrono::steady_clock::time_point last_call_;
        std::atomic_bool first_call_;
        std::mutex mtx_;

        long getDt() {
            long dt;
            auto now = std::chrono::steady_clock::now();
            dt = std::chrono::duration_cast<T>(now - last_call_).count();
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
        PID(X kp = 1.0, X kd = 1.0, X ki = 1.0) : ki_{ki}, kd_{kd}, kp_{kp}, kaw_{0}, min_{0}, max_{0}, is_bound_{false}
        {
            static_assert(is_chrono_duration<T>::value, "T not derived from std::chrono::duration");
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
        PID(X kp, X kd, X ki, X kaw, X min, X max) : ki_{ki}, kd_{kd}, kp_{kp}, kaw_{kaw}, min_{min}, max_{max}, is_bound_{true}
        {
            static_assert(is_chrono_duration<T>::value, "T not derived from std::chrono::duration");
        }

        /**
         * Method to get control value 
         * @param is The is state of the system
         * @param desired The desired state
         */
        X control(X is, X desired) 
        {
             if(first_call_){
                last_call_ = std::chrono::steady_clock::now();
                first_call_ = false;
            }
            auto err = is - desired;
            std::lock_guard<std::mutex> guard(mtx_);
            auto dt = getDt();
            integral_ += err * dt;
            auto out = kp_ * err + kd_ * (err - last_error_) / dt + ki_ * integral_;
            last_error_ = err;
            if(is_bound_) {
                auto old = out;
                out = BOUND<double>(out, min_, max_);
                integral_ += kaw_ * (out - old);
            }
            return out;
        }
    };
} // namespace drone

#endif