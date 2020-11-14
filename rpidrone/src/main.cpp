#include "loop.hpp"
#include "easylogging++.h"

INITIALIZE_EASYLOGGINGPP

void initLogger() {
    // Load configuration from file
    el::Configurations conf("/home/pi/mnt/RPiDrone/log.conf");
    // Reconfigure single logger
    el::Loggers::reconfigureLogger("default", conf);
    // Actually reconfigure all loggers instead
    el::Loggers::reconfigureAllLoggers(conf);
    // Now all the loggers will use configuration from file
}

int main() {
    initLogger();
    LOG(DEBUG) << "Loading config.json";
    drone::Loop l ("/home/pi/mnt/RPiDrone/config.json");
    LOG(DEBUG) << "Drone startup";
    l.startupDrone();
    LOG(DEBUG) << "Drone startup completed; starting main loop";
    while(1) {
        l.loop();
    }
    return 0;
}

