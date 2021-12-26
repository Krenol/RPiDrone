#ifndef DRONE_LOGS_LOGS_H
#define DRONE_LOGS_LOGS_H

#include <string>
#include "easylogging++.h"
#include <ctime>
#include "log_definition.hpp"

INITIALIZE_EASYLOGGINGPP

namespace drone
{
    namespace logs
    {
        class Logs
        {
        public:
            void backupOldLogFiles(const std::string &logDir, const std::string &oldLogDir, const std::string &confDir, const std::string &logConf)
            {
                auto t = std::to_string(std::time(0));
                mvFile(logDir + "/flightcontroller.csv", oldLogDir + "/flightcontroller_" + t + ".csv");
                mvFile(logDir + "/network.log", oldLogDir + "/network_" + t + ".log");
                mvFile(logDir + "/default.log", oldLogDir + "/default_" + t + ".log");
                mvFile(logDir + "/power.csv", oldLogDir + "/power_" + t + ".csv");
                mvFile(logDir + "/exec.csv", oldLogDir + "/exec_" + t + ".csv");
                mvFile(logDir + "/rpi.csv", oldLogDir + "/rpi_" + t + ".csv");
                //enable multi loggers
                el::Loggers::addFlag(el::LoggingFlag::MultiLoggerSupport);
                // configure all loggers
                el::Loggers::configureFromGlobal((confDir + "/" + logConf).c_str());
            }

            void initLogFileHeaders() {
                #if defined(EXEC_TIME_LOG)
                EXEC_LOG(DEBUG) << "datetime;level;t_exec";
                #endif
                #if defined(FLIGHTCONTROLLER_LOGS)
                FLIGHT_LOG(DEBUG) << "datetime;level;yaw;pitch;roll;t_exec;ax;ay;az;rf_t,;rb_t;lf_t;lb_t;throttle";
                #endif
                #if defined(RPI_LOGS)
                RPI_LOG(DEBUG) << "datetime;level;yaw_vel;pitch_angle;roll_angle;throttle";
                #endif
            }

        private:
            static void mvFile(const std::string &file, const std::string &newFile)
            {
                try
                {
                    std::ifstream in(file.c_str(), std::ios::in | std::ios::binary);
                    if (in.good())
                    {
                        std::ofstream out(newFile.c_str(), std::ios::out | std::ios::binary);
                        out << in.rdbuf();
                        std::remove(file.c_str());
                    }
                }
                catch (...)
                {
                }
            }
        };
    }
} // namespace drone

#endif