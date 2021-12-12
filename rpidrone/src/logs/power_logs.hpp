#ifndef DRONE_LOGS_POWER_LOGS_H
#define DRONE_LOGS_POWER_LOGS_H

#include "cpu_log.hpp"
#include "memory_log.hpp"
#include "easylogging++.h"
#include <unistd.h>
#include <thread>
#include <atomic>

#define POWER_LOG(LEVEL) CLOG(LEVEL, "power")

namespace drone
{
    namespace logs
    {
        class PowerLogs
        {
        private:
            std::atomic_bool keepRunning_;
            std::thread logThread_;

        public:
            void stopPowerLogThread() {
                stopPowerLogLoop();
                logThread_.join();
            }

            void startPowerLogThread()
            {
                stopPowerLogThread();
                logThread_ = std::thread([this] () {
                    startPowerLogLoop();                    
                });
            }

            void stopPowerLogLoop() {
                keepRunning_ = false;
            }

            void startPowerLogLoop()
            {
                keepRunning_ = true;
                while (keepRunning_)
                {
                    POWER_LOG(INFO) << drone::logs::systemCpuConsumption() << ";" << drone::logs::getCpuConsumption() << ";" << drone::logs::getMemoryConsumption() << ";" << drone::logs::getVirtualMemoryConsumption();
                    usleep(100000);
                }
            }

            PowerLogs()
            {
                drone::logs::initCpuLog();
                POWER_LOG(DEBUG) << "datetime;level;sys_cpu;proc_cpu;memory;virtual_memory";
            }
        };
    }
} // namespace drone

#endif