#ifndef DRONE_LOGS_POWER_LOGS_H
#define DRONE_LOGS_POWER_LOGS_H

#include "cpu_log.hpp"
#include "memory_log.hpp"
#include "easylogging++.h"
#include <unistd.h>
#define POWER_LOG(LEVEL) CLOG(LEVEL, "power")

namespace drone
{
    namespace logs
    {
        class PowerLogs
        {
        public:
            void init(const bool &run)
            {
                drone::logs::initCpuLog();
                POWER_LOG(DEBUG) << "datetime;level;sys_cpu;proc_cpu;memory;virtual_memory";
                while (run)
                {
                    POWER_LOG(INFO) << drone::logs::systemCpuConsumption() << ";" << drone::logs::getCpuConsumption() << ";" << drone::logs::getMemoryConsumption() << ";" << drone::logs::getVirtualMemoryConsumption();
                    usleep(100000);
                }
            }
        };
    }
} // namespace drone

#endif