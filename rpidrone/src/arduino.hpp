#ifndef DRONE_ARDUINO_H
#define DRONE_ARDUINO_H

#include <string>
#include "structs/config/config.hpp"

namespace drone
{
    class Arduino {
        public:
            Arduino(const std::string &serial, int baudrate);

            void init(const structs::config::Config &config);

            void serialSend(const std::string &msg);

            void serialRead(char *msg, const char eol = '\n');

            int availableData();

            void clearReceiverBuffer();

            void serialFlushBuffer();
        private:
            const int fd_ard_;
    };

} // namespace drone

#endif