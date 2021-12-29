#ifndef DRONE_LOGS_POWER_LOGS_THREAD_H
#define DRONE_LOGS_POWER_LOGS_THREAD_H

#include "power_logs.hpp"
#include <thread>
#include <atomic>

namespace drone
{
    namespace logs
    {
        class PowerLogsThread
        {
        private:
            std::thread logThread_;
            PowerLogs powerLogs_;

        public:
            void stopPowerLogThread() {
                powerLogs_.stopPowerLogLoop();
                logThread_.join();
            }

            void startPowerLogThread()
            {
                logThread_ = std::thread([this] () {
                    powerLogs_.startPowerLogLoop();                    
                });
            }

           
        };
    }
} // namespace drone

#endif