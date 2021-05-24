#ifndef DRONE_ARDUINO_H
#define DRONE_ARDUINO_H

#include <string>
#include "pin/pins.hpp"

#define OUT_MSG_SIZE 90
#define CONTROL_TOKEN "<S>"

namespace drone
{
    class Arduino{
        public:
            Arduino(const std::string &serial, int baudrate, int resetPin);

            void init(const std::string &conf);

            void serialSend(const std::string &msg);

            void serialRead(char *msg, const char EOL = '\n');

            void reset();

        private:
            const int fd_ard_;
            std::unique_ptr<pin::DigitalPin> resetPin_;
            bool initialized_ = false;

    };

} // namespace drone

#endif