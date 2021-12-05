#ifndef DRONE_ARDUINO_H
#define DRONE_ARDUINO_H

#include <string>
#include "structs/config/config.hpp"

#define OUT_MSG_SIZE 90
#define CONTROL_TOKEN "<S>"
#define ACK_TOKEN "<A>"

namespace drone
{
    class Flightcontroller {
        public:
            Flightcontroller(const std::string &serial, int baudrate);

            void init(const structs::config::Config &config);

            void serialSend(const std::string &msg);

            void serialRead(char *msg, const char eol = '\n');

            int availableData();

            void clearReceiverBuffer();

        private:
            const int fd_ard_;
            bool initialized_ = false;

            void configureFlightcontroller(const std::string &config);
            void awaitSuccessfulSetup();
    };

} // namespace drone

#endif