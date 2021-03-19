#include <mutex>
#include <chrono>
#include <atomic>
#include "misc.hpp"
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

        long getDt();

    public:
        /**
         * Constructor for PID
         * @param kp Value for proportional part
         * @param kd Value for differential part
         * @param ki Value for integral part
         * @param kaw Anti windup param for controller
         */
        PID(T kp = 1.0, T kd = 1.0, T ki = 1.0);

        /**
         * Constructor for PID
         * @param kp Value for proportional part
         * @param kd Value for differential part
         * @param ki Value for integral part
         * @param kaw Anti windup param for controller
         * @param min Min value of the controller
         * @param max max value of the controller
         */
        PID(T kp, T kd, T ki, T kaw, T min, T max);

        /**
         * Method to get control value 
         * @param is The is state of the system
         * @param desired The desired state
         */
        T control(T is, T desired);
    };
} // namespace drone

#endif