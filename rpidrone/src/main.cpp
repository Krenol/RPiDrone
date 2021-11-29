#include "loop.hpp"
#include "logs/easylogging++.h"
#include "logs/logs.hpp"
#include "globals.hpp"
#include "misc.hpp"
#include <fstream>
#include "parsers/flightcontroller/config_parser.hpp"
#include <string>
#include "parsers/json/config_parser.hpp" 
#include "structs/config/config.hpp"
#include "arduino.hpp"
#include "websocket.hpp"
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#if defined(POWER_LOGS)
#include "logs/power_logs.hpp"
#endif
#if defined(CONF_API_MODE)
#include <cstdlib>
#endif

using json = nlohmann::json;

//INITIALIZE_EASYLOGGINGPP

static void loadConfig(const std::string &conf_file, drone::Config &config, std::string &fc_conf){
    std::ifstream ifs(conf_file);
    config = json::parse(ifs);
    drone::parseConfigForFlightcontroller(config, fc_conf);
}

int main() {
    bool run = true;
    drone::Config config;
    std::string fc_conf;
    drone::logs::Logs log;
    log.init(LOG_DIR, OLD_LOG_DIR, CONF_DIR, LOG_CONF);
    drone::Loop l;
    #if defined(CONF_API_MODE)
    LOG(INFO) << "Starting API Server....";
    std:: string cmd = "/usr/bin/python3 " + HOME_DIR + "/api/main.py &";
    int pid;
	if(pid = system(cmd.c_str()) < 0) {
        LOG(ERROR) << "API Server did not started successfully";
    } else {
        LOG(INFO) << "API Server started successfully with PID: " << pid;
    }
    
    #endif
    #if defined(POWER_LOGS)
    std::thread thrd([&run] () {
        drone::logs::PowerLogs pwLogs;
        pwLogs.init(run); 
    });
    #endif
    LOG(INFO) << "Loading " << CONF_FILE;
    loadConfig(CONF_DIR + "/" + CONF_FILE, config, fc_conf);
    LOG(INFO) << "Starting Websocket Server";
    drone::Websocket websocket(config.server.queue_size);
    websocket.init(config.server.context, config.server.port);
    websocket.startWebsocketThread();
    LOG(INFO) << "Started Websocket Server sucessfully";
    LOG(INFO) << "Starting the drone flightcontroller & drone setup";
    drone::Arduino fc(config.flightcontroller.port, config.flightcontroller.baudrate);
    LOG(INFO) << "init flightcontroller conf";
    fc.init(fc_conf);
    LOG(INFO) << "Drone & flightcontroller startup completed; starting main loop";
    while (1)
    {
        try {
            if(websocket.hasConnection()) {
                l.loop(websocket, fc, config);
                #if defined(SIMULATOR_MODE)
                LOG(INFO) << "---SIMULATOR MODE ACTIVE -> LOADING NEW CONF---";
                loadConfig(CONF_DIR + "/" + CONF_FILE, config, fc_conf);
                fc.init(fc_conf);
                #endif 
                websocket.disconnectConnectedClient();
            } else {
                usleep(1000 * 1000);
            }
        } catch(const std::exception &exc) {
            LOG(ERROR) << exc.what();
        } 
    }
    run = false;
    #if defined(POWER_LOGS)
    thrd.join();
    #endif
    return 0;
}


void wsTest() {
    drone::Websocket ws(10);
    ws.init("/", 8000);
    ws.startWebsocketThread();
    std::string msg;
    srand (time(NULL));
    while(true) 
    {
        if(ws.hasConnection()) {
            auto v1 = rand() % 100;         
            ws.writeMessage("{\"gps\":{},\"joystick\":{\"degrees\":0,\"offset\":0,\"rotation\":0},\"throttle\":" + std::to_string(v1) + "}");
            if(ws.hasMessages()) {
                ws.getMessage(msg);
                LOG(INFO) << "received: " << msg;
            }
            
        } else {
            LOG(INFO) << "no conn";
        }
            
        usleep(1000 * 1000);
    }
}