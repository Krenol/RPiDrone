#include "loop.hpp"
#include "logs/easylogging++.h"
#include "logs/logs.hpp"
#include "globals.hpp"
#include "misc.hpp"
#include <fstream>
#include "parsers/arduino_config_parser.hpp"
#include <string>
#include "parsers/json_config_parser.hpp" 
#include "rpisocket/rpisocket.hpp"
#include "connection.hpp"
#include "arduino.hpp"
#include "websocket.hpp"
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
    drone::parse_config(config, fc_conf);
}

int main() {
    // bool run = true;
    // drone::Config config;
    // std::string fc_conf;
    // drone::logs::Logs log;
    // log.init(LOG_DIR, OLD_LOG_DIR, CONF_DIR, LOG_CONF);
    // drone::Loop l;
    // #if defined(CONF_API_MODE)
    // LOG(INFO) << "Starting API Server....";
    // std:: string cmd = "/usr/bin/python3 " + HOME_DIR + "/api/main.py &";
    // int pid;
	// if(pid = system(cmd.c_str()) < 0) {
    //     LOG(ERROR) << "API Server did not started successfully";
    // } else {
    //     LOG(INFO) << "API Server started successfully with PID: " << pid;
    // }
    
    // #endif
    // #if defined(POWER_LOGS)
    // std::thread thrd([&run] () {
    //     drone::logs::PowerLogs pwLogs;
    //     pwLogs.init(run); 
    // });
    // #endif
    // LOG(INFO) << "Loading " << CONF_FILE;
    // loadConfig(CONF_DIR + "/" + CONF_FILE, config, fc_conf);
    // LOG(INFO) << "Starting Wifi Server";
    // rpisocket::WiFiServer server(config.server.port, config.server.bytes);
    // LOG(INFO) << "Drone startup";
    // drone::Arduino fc(config.flightcontroller.port, config.flightcontroller.baudrate);
    // LOG(INFO) << "init FC conf";
    // fc.init(fc_conf);
    // LOG(INFO) << "Drone startup completed; starting main loop";
    // while (1)
    // {
    //     try {
    //         l.loop(server, fc, config);
    //         #if defined(SIMULATOR_MODE)
    //         LOG(INFO) << "---SIMULATOR MODE ACTIVE -> LOADING NEW CONF---";
    //         loadConfig(CONF_DIR + "/" + CONF_FILE, config, fc_conf);
    //         fc.init(fc_conf);
    //         #endif 
    //     } catch(const std::exception &exc) {
    //         LOG(ERROR) << exc.what();
    //     } 
    // }
    // run = false;
    // thrd.join();
    drone::Websocket ws(10);
    ws.init("/", 80);
    while(true) 
    {
        if(ws.hasConnection()) {
            ws.writeMsg("hi");
        } else {
            LOG(INFO) << "no conn";
        }
            
        usleep(1000 * 1000);
    }
    return 0;
}
