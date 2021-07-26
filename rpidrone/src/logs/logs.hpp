#ifndef DRONE_LOGS_LOGS_H
#define DRONE_LOGS_LOGS_H

#include <string>
#include "easylogging++.h"
#include <ctime>

namespace drone
{
    namespace logs
    {
        class Logs
        {
        public:
            void init(const std::string &logDir, const std::string &oldLogDir, const std::string &confDir, const std::string &logConf)
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