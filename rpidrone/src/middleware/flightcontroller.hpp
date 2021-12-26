#ifndef DRONE_MIDDLEWARE_FC_H
#define DRONE_MIDDLEWARE_FC_H

#include "../arduino.hpp"
#include <memory>
#include <string>
#include <chrono>
#include "../structs/middleware/input.hpp"
#include "../structs/middleware/output.hpp"
#include "../structs/config/config.hpp"

namespace drone {
    namespace middleware {
        class Flightcontroller {
            private:
                std::unique_ptr<Arduino> arduino_;
                structs::config::Config config_;
                void configureFlightcontroller(const structs::config::Config &config);
                void parseConfigStructToString(const structs::config::Config &config, std::string &configStr);
                void sendConfigurationToFlightcontroller(const std::string &config);
                void checkForConfigurationTimeout(const std::chrono::steady_clock::time_point &startTime);
                void awaitSuccessfulSetup();
                bool tokenExistsInString(const std::string &string, const std::string &token);
            
            public:
                Flightcontroller(std::unique_ptr<Arduino> arduino, const structs::config::Config &config);

                void sendToFlightcontroller(const structs::middleware::Input &inMsg);

                void receiveFromFlightcontroller(structs::middleware::Output &outMsg);

                int bytesOfAvailableData();

                void clearReceiverBuffer();
        };
    }
}

#endif