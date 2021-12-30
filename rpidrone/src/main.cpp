#include "logs/logs.hpp"
#include "globals.hpp"
#include <string>
#include "api/api_server.hpp"
#include "coordinator.hpp"
#if defined(POWER_LOGS)
#include "logs/power_logs_thread.hpp"
#endif
#if defined(CONF_API_MODE)
#include <cstdlib>
#endif

static void initLogs() {
    drone::logs::Logs log;
    log.backupOldLogFiles(LOG_DIR, OLD_LOG_DIR, CONF_DIR, LOG_CONF);
    log.initLogFileHeaders();
}

int main() {
    initLogs();
    #if defined(CONF_API_MODE)
    LOG(INFO) << "Starting API Server....";
    drone::api::ApiServer apiServer;
    apiServer.startApiServer();
    #endif
    #if defined(POWER_LOGS)
    drone::logs::PowerLogsThread pwLogs;
    pwLogs.startPowerLogThread();
    #endif
    drone::Coordinator coordinator(CONF_DIR + "/" + CONF_FILE);
    coordinator.coordinateDrone();
    #if defined(POWER_LOGS)
    pwLogs.stopPowerLogThread();
    #endif
    #if defined(CONF_API_MODE)
    apiServer.stopApiServer();
    #endif
    return 0;
}

