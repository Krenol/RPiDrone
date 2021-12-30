#ifndef DRONE_COORDINATOR_H
#define DRONE_COORDINATOR_H

#include <string>
#include "structs/config/config.hpp"
#include <memory>
#include "middleware/client.hpp"
#include "middleware/flightcontroller.hpp"
#include "connected_client_handler.hpp"

namespace drone
{
    class Coordinator
    {
    private:
        const std::string configFile_;
        std::unique_ptr<drone::middleware::Client> middlewareClient_;
        std::unique_ptr<drone::middleware::Flightcontroller> middlewareFlightcontroller_;
        structs::config::Config config_;
        ConnectedClientHandler connectedClientHandler_;

        void handleConnectedClient();
        void reloadConfigAndFlightcontroller();
        void handleNoConnectedClient();

    public:
        Coordinator(const std::string &configFile);
        void coordinateDrone();
    };

}

#endif