#include "loop.hpp"
#include "easylogging++.h"

INITIALIZE_EASYLOGGINGPP

static void initLogging()
{
    //enable multi loggers
    el::Loggers::addFlag(el::LoggingFlag::MultiLoggerSupport);
    // configure all loggers
    el::Loggers::configureFromGlobal("/home/pi/mnt/RPiDrone/conf/log.conf");
}

int main() {
    initLogging();
    LOG(INFO) << "Loading config.json";
    drone::Loop l ("/home/pi/mnt/RPiDrone/conf/config.json");
    LOG(INFO) << "Drone startup";
    l.startupDrone();
    LOG(INFO) << "Drone startup completed; starting main loop";
    while(1) {
        l.loop();
    }
    return 0;
}
