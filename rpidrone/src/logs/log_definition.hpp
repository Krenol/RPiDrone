#ifndef DRONE_LOGS_LOG_DEFINITION_H
#define DRONE_LOGS_LOG_DEFINITION_H

#define NETWORK_LOG(LEVEL) CLOG(LEVEL, "network")  //define network log
#if defined(EXEC_TIME_LOG)
#define EXEC_LOG(LEVEL) CLOG(LEVEL, "exec") //define exec time log
#endif
#if defined(FLIGHTCONTROLLER_LOGS)
#define FLIGHT_LOG(LEVEL) CLOG(LEVEL, "flightcontroller")
#endif
#if defined(RPI_LOGS)
#define RPI_LOG(LEVEL) CLOG(LEVEL, "rpi")
#endif

#endif