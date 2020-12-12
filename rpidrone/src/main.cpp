#include "loop.hpp"
#include "easylogging++.h"
#include "globals.hpp"
#include <cstdio>
#include <fstream>
#include <iostream>
#include <ios>
#include <cstdio>
#include <ctime>

INITIALIZE_EASYLOGGINGPP

static void mvFile(const std::string& file, const std::string& newFile){
    std::ifstream in(file.c_str(), std::ios::in | std::ios::binary);
    if(in.good()) {
        std::ofstream out(newFile.c_str(), std::ios::out | std::ios::binary);
        out << in.rdbuf();
        std::remove(file.c_str());
    }
}

static void initLogging()
{
    mvFile(LOG_DIR + "/pid.csv", LOG_DIR + "/pid_" + std::to_string(std::time(0)) + ".csv");
    //enable multi loggers
    el::Loggers::addFlag(el::LoggingFlag::MultiLoggerSupport);
    // configure all loggers
    el::Loggers::configureFromGlobal((CONF_DIR + "/" + LOG_CONF).c_str());
}


int main() {
    initLogging();
    LOG(INFO) << "Loading " << CONF_FILE;
    drone::Loop l (CONF_DIR + "/" + CONF_FILE);
    LOG(INFO) << "Drone startup";
    l.startupDrone();
    LOG(INFO) << "Drone startup completed; starting main loop";

    l.loop();
    
    return 0;
}
