#include "loop.hpp"
#include "logs/easylogging++.h"
#include "globals.hpp"
#include <cstdio>
#include <fstream>
#include <iostream>
#include <ios>
#include <ctime>
#if defined(POWER_LOGS)
#include "logs/cpu_log.hpp"
#include "logs/memory_log.hpp"
#define POWER_LOG(LEVEL) CLOG(LEVEL, "power")  
#endif

INITIALIZE_EASYLOGGINGPP

static void mvFile(const std::string& file, const std::string& newFile){
    try {
        std::ifstream in(file.c_str(), std::ios::in | std::ios::binary);
        if(in.good()) {
            std::ofstream out(newFile.c_str(), std::ios::out | std::ios::binary);
            out << in.rdbuf();
            std::remove(file.c_str());
        }
    } catch(...) {

    }
    
}

static void initLogging()
{
    auto t = std::to_string(std::time(0));
    mvFile(LOG_DIR + "/flightcontroller.csv", OLD_LOG_DIR + "/flightcontroller_" + t + ".csv");
    mvFile(LOG_DIR + "/network.log", OLD_LOG_DIR + "/network_" + t + ".log");
    mvFile(LOG_DIR + "/default.log", OLD_LOG_DIR + "/default_" + t + ".log");
    mvFile(LOG_DIR + "/power.csv", OLD_LOG_DIR + "/power_" + t + ".csv");
    mvFile(LOG_DIR + "/exec.csv", OLD_LOG_DIR + "/exec_" + t + ".csv");
    //enable multi loggers
    el::Loggers::addFlag(el::LoggingFlag::MultiLoggerSupport);
    // configure all loggers
    el::Loggers::configureFromGlobal((CONF_DIR + "/" + LOG_CONF).c_str());
}

void initPowerLogs(const bool& run) {
    drone::logs::initCpuLog();
    POWER_LOG(DEBUG) << "datetime;level;sys_cpu;proc_cpu;memory;virtual_memory";
    while(run) {
        POWER_LOG(INFO) << drone::logs::systemCpuConsumption() << ";" << drone::logs::getCpuConsumption() << ";" << drone::logs::getMemoryConsumption() << ";" << drone::logs::getVirtualMemoryConsumption();
        usleep(100000);
    }
}

int main() {
    bool run = true;
    initLogging();
    #if defined(POWER_LOGS)
    std::thread thrd([&run] () {initPowerLogs(run); });
    #endif
    LOG(INFO) << "Loading " << CONF_FILE << " and starting up drone";
    drone::Loop l (CONF_DIR + "/" + CONF_FILE);
    LOG(INFO) << "Drone startup completed; starting main loop";
    //catchSigActions();
    l.loop();
    run = false;
    thrd.join();
    
    return 0;
}
