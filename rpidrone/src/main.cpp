#include "loop.hpp"
#include "logs/easylogging++.h"
#include "logs/logs.hpp"
#include "globals.hpp"
#include "misc.hpp"
#if defined(POWER_LOGS)
#include "logs/power_logs.hpp"
#endif

INITIALIZE_EASYLOGGINGPP

int main() {
    bool run = true;
    drone::logs::Logs log;
    log.init(LOG_DIR, OLD_LOG_DIR, CONF_DIR, LOG_CONF);
    #if defined(POWER_LOGS)
    std::thread thrd([&run] () {
        drone::logs::PowerLogs pwLogs;
        pwLogs.init(run); 
    });
    #endif
    LOG(INFO) << "Loading " << CONF_FILE << " and starting up drone";
    drone::Loop l (CONF_DIR + "/" + CONF_FILE);
    LOG(INFO) << "Drone startup completed; starting main loop";
    //catchSigActions();
    l.loop();
    run = false;
    thrd.join();
    
    return 0;
}
