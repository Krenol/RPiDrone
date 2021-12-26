#include "loop.hpp"
#include "logs/easylogging++.h"
#include "logs/logs.hpp"
#include "globals.hpp"
#include "config.hpp"
#include <string>
#include "structs/config/config.hpp"
#include "flightcontroller.hpp"
#include "websocket.hpp"
#include "api/api_server.hpp"
#if defined(POWER_LOGS)
#include "logs/power_logs_thread.hpp"
#endif
#if defined(CONF_API_MODE)
#include <cstdlib>
#endif

using json = nlohmann::json;

int main() {
    bool run = true;
    drone::structs::config::Config config;
    drone::logs::Logs log;
    log.backupOldLogFiles(LOG_DIR, OLD_LOG_DIR, CONF_DIR, LOG_CONF);
    log.initLogFileHeaders();
    drone::Loop l;
    #if defined(CONF_API_MODE)
    LOG(INFO) << "Starting API Server....";
    drone::api::ApiServer apiServer;
    apiServer.startApiServer();
    #endif
    #if defined(POWER_LOGS)
    drone::logs::PowerLogsThread pwLogs;
    pwLogs.startPowerLogThread();
    #endif
    LOG(INFO) << "Loading " << CONF_FILE;
    drone::Config::loadConfig(CONF_DIR + "/" + CONF_FILE, config);
    LOG(INFO) << "Starting Websocket Server";
    drone::Websocket websocket(config.server.queue_size);
    websocket.init(config.server.context, config.server.port);
    websocket.startWebsocketThread();
    LOG(INFO) << "Started Websocket Server sucessfully";
    LOG(INFO) << "Starting the drone flightcontroller & drone setup";
    drone::Flightcontroller fc(config.flightcontroller.port, config.flightcontroller.baudrate);
    LOG(INFO) << "init flightcontroller conf";
    fc.init(config);
    LOG(INFO) << "Drone & flightcontroller startup completed; starting main loop";
    while (1)
    {
        try {
            if(websocket.hasConnection()) {
                l.loop(websocket, fc, config);
                #if defined(SIMULATOR_MODE)
                LOG(INFO) << "---SIMULATOR MODE ACTIVE -> LOADING NEW CONF---";
                drone::Config::loadConfig(CONF_DIR + "/" + CONF_FILE, config);
                fc.init(config);
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
    pwLogs.stopPowerLogThread();
    #endif
    #if defined(CONF_API_MODE)
    apiServer.stopApiServer();
    #endif
    return 0;
}

