#include "loop.hpp"
#include "easylogging++.h"
#include "globals.hpp"
#include <cstdio>

INITIALIZE_EASYLOGGINGPP

static void initLogging()
{
    //enable multi loggers
    el::Loggers::addFlag(el::LoggingFlag::MultiLoggerSupport);
    // configure all loggers
    el::Loggers::configureFromGlobal((CONF_DIR + "/log.conf").c_str());
}

static void removeLogs() {
    std::remove((LOG_DIR + "/pid.csv").c_str());
    std::remove((LOG_DIR + "/network.log").c_str());
    std::remove((LOG_DIR + "/default.log").c_str());
    std::remove((LOG_DIR + "/controls.log").c_str());
}

int main() {
    removeLogs();
    initLogging();
    LOG(INFO) << "Loading config.json";
    drone::Loop l (CONF_DIR + "/config.json");
    LOG(INFO) << "Drone startup";
    l.startupDrone();
    LOG(INFO) << "Drone startup completed; starting main loop";
    while(1) {
        l.loop();
    }
    return 0;
}
