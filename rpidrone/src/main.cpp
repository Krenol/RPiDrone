#include "connected_client_handler.hpp"
#include "logs/easylogging++.h"
#include "logs/logs.hpp"
#include "globals.hpp"
#include "config.hpp"
#include <string>
#include "structs/config/config.hpp"
#include "arduino.hpp"
#include "websocket.hpp"
#include "api/api_server.hpp"
#include "middleware/client.hpp"
#include "middleware/flightcontroller.hpp"
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
    drone::ConnectedClientHandler connectedClientHandler;
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
    auto websocket = std::make_unique<drone::Websocket>(config.server.queue_size, config.server.context, config.server.port);
    websocket->startWebsocketThread();
    LOG(INFO) << "Started Websocket Server sucessfully";
    LOG(INFO) << "Starting the drone flightcontroller & drone setup";
    auto arduino = std::make_unique<drone::Arduino>(config.flightcontroller.port, config.flightcontroller.baudrate);
    drone::middleware::Client clientMw(std::move(websocket));
    drone::middleware::Flightcontroller fcMw(std::move(arduino), config);
    while (1)
    {
        try {
            if(websocket->hasConnection()) {
                connectedClientHandler.handleConnectedClient(clientMw, fcMw, config);
                #if defined(SIMULATOR_MODE)
                LOG(INFO) << "---SIMULATOR MODE ACTIVE -> LOADING NEW CONF---";
                drone::Config::loadConfig(CONF_DIR + "/" + CONF_FILE, config);
                arduino = std::make_unique<drone::Arduino>(config.flightcontroller.port, config.flightcontroller.baudrate);
                fcMw = drone::middleware::Flightcontroller(std::move(arduino), config);
                #endif 
                websocket->disconnectConnectedClient();
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

