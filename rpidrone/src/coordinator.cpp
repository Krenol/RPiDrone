#include "coordinator.hpp"
#include "logs/log_definition.hpp"
#include "config.hpp"

namespace drone
{
    void Coordinator::handleConnectedClient() 
    {
        connectedClientHandler_.handleConnectedClient(middlewareClient_, middlewareFlightcontroller_, config_);
        #if defined(SIMULATOR_MODE)
        reloadConfigAndFlightcontroller();
        #endif
        middlewareClient_->disconnectConnectedClient();
    }
    
    void Coordinator::reloadConfigAndFlightcontroller() 
    {
        LOG(INFO) << "---SIMULATOR MODE ACTIVE -> LOADING NEW CONF---";
        Config::loadConfig(configFile_, config_);
        auto arduino = std::make_unique<Arduino>(config_.flightcontroller.port, config_.flightcontroller.baudrate);
        middlewareFlightcontroller_ = std::make_unique<middleware::Flightcontroller>(std::move(arduino), config_);
        LOG(INFO) << "---SIMULATOR MODE ACTIVE -> NEW CONF LOADED---";
    }
    
    void Coordinator::handleNoConnectedClient() 
    {
        usleep(1000 * 1000);
    }

    Coordinator::Coordinator(const std::string &configFile) : configFile_{configFile}
    {
        Config::loadConfig(configFile_, config_);
        LOG(INFO) << "Starting the drone flightcontroller & drone setup";
        auto arduino = std::make_unique<Arduino>(config_.flightcontroller.port, config_.flightcontroller.baudrate);
        middlewareFlightcontroller_ = std::make_unique<middleware::Flightcontroller>(std::move(arduino), config_);
        LOG(INFO) << "Starting Websocket Server";
        auto websocket = std::make_unique<Websocket>(config_.server.queue_size, config_.server.context, config_.server.port);
        websocket->startWebsocketThread();
        middlewareClient_ = std::make_unique<middleware::Client>(std::move(websocket));
        LOG(INFO) << "Successfully started Websocket Server";
    }

    void Coordinator::coordinateDrone()
    {
        while (1)
        {
            try
            {
                if (middlewareClient_->clientConnectionAvailable())
                {
                    handleConnectedClient();
                }
                else
                {
                    handleNoConnectedClient();
                }
            }
            catch (const std::exception &exc)
            {
                LOG(ERROR) << exc.what();
            }
        }
    }
}