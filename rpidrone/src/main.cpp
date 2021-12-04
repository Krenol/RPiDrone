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
#include "flightcontroller.hpp"
#include "websocket.hpp"
#include "api/api_server.hpp"
#if defined(POWER_LOGS)
#include "logs/power_logs.hpp"
#endif
#if defined(CONF_API_MODE)
#include <cstdlib>
#endif

using json = nlohmann::json;

static void loadConfig(const std::string &conf_file, drone::structs::config::Config &config, std::string &fc_conf){
    std::ifstream ifs(conf_file);
    auto j = json::parse(ifs);
    drone::parsers::json::ConfigParser::parseJsonConfigToStruct(j, config);
    drone::parsers::flightcontroller::ConfigParser::parseFlightcontrollerConfig(config, fc_conf);
}

int main() {
    bool run = true;
    drone::structs::config::Config config;
    std::string fc_conf;
    drone::logs::Logs log;
    log.init(LOG_DIR, OLD_LOG_DIR, CONF_DIR, LOG_CONF);
    drone::Loop l;
    #if defined(CONF_API_MODE)
    LOG(INFO) << "Starting API Server....";
    drone::api::ApiServer apiServer;
    std:: string cmd = "/usr/bin/python3 " + HOME_DIR + "/api/main.py &";
    try {
        apiServer.startApiServer(cmd);
        LOG(INFO) << "API Server started successfully";
    } catch(...) {
        LOG(ERROR) << "API Server did not started successfully";
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
    drone::Flightcontroller fc(config.flightcontroller.port, config.flightcontroller.baudrate);
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

