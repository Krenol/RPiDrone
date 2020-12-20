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
    mvFile(LOG_DIR + "/network.log", LOG_DIR + "/network_" + std::to_string(std::time(0)) + ".log");
    //enable multi loggers
    el::Loggers::addFlag(el::LoggingFlag::MultiLoggerSupport);
    // configure all loggers
    el::Loggers::configureFromGlobal((CONF_DIR + "/" + LOG_CONF).c_str());
}

// void sig_handler(int s){
//     printf("Caught signal %d\n",s);
//     for(int i < 0; i < 53; i++){
        
//     }
//     pin::terminateGPIOs();
//     exit(0); 
// }

// void catchSigActions(){
//     struct sigaction sigIntHandler;
//     sigIntHandler.sa_handler = sig_handler;
//     sigemptyset(&sigIntHandler.sa_mask);
//     sigIntHandler.sa_flags = 0;
//     sigaction(SIGINT, &sigIntHandler, NULL);
// }

int main() {
    initLogging();
    LOG(INFO) << "Loading " << CONF_FILE;
    drone::Loop l (CONF_DIR + "/" + CONF_FILE);
    LOG(INFO) << "Drone startup";
    l.startupDrone();
    LOG(INFO) << "Drone startup completed; starting main loop";
    //catchSigActions();
    l.loop();
    
    return 0;
}
