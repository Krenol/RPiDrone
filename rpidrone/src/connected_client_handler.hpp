#ifndef DRONE_CONNECTED_CLIENT_HANDLER_H
#define DRONE_CONNECTED_CLIENT_HANDLER_H

#include <memory>
#include <string>
#include "structs/config/config.hpp"
#include "middleware/client.hpp"
#include "middleware/flightcontroller.hpp"


using json = nlohmann::json;

namespace drone {
    class ConnectedClientHandler  {
        private:
            structs::middleware::Input input_;
            structs::middleware::Output output_;
            void handleClientMessage(middleware::Client &clientMw, middleware::Flightcontroller &fcMw);
            bool handleFlightcontrollerMessage(middleware::Client &clientMw, middleware::Flightcontroller &fcMw);
            void handleClientDisconnect(middleware::Flightcontroller &fcMw, const structs::config::Config &config);
            void setInputValuesForDisconnect(const structs::config::Config &config);

        public:
            void handleConnectedClient(middleware::Client &clientMw, middleware::Flightcontroller &fcMw, const structs::config::Config &config);
            
    };
}

#endif