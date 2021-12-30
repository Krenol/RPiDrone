#include "connected_client_handler.hpp"
#include <unistd.h>
#include "logs/easylogging++.h"
#include "parsers/flightcontroller/input_parser.hpp"
#include "parsers/flightcontroller/output_parser.hpp"
#include "misc/json.hpp"
#include "logs/log_definition.hpp"

namespace drone
{
    void ConnectedClientHandler::handleClientMessage(std::unique_ptr<middleware::Client> &clientMw, std::unique_ptr<middleware::Flightcontroller> &fcMw) 
    {
        clientMw->receiveFromClient(input_);
        fcMw->sendToFlightcontroller(input_);
    }
    
    bool ConnectedClientHandler::handleFlightcontrollerMessage(std::unique_ptr<middleware::Client> &clientMw, std::unique_ptr<middleware::Flightcontroller> &fcMw) 
    {
        fcMw->receiveFromFlightcontroller(output_);
        return clientMw->sendToClient(output_);
    }
    
    void ConnectedClientHandler::handleClientDisconnect(std::unique_ptr<middleware::Flightcontroller> &fcMw, const structs::config::Config &config) 
    {
        setInputValuesForDisconnect(config);
        fcMw->sendToFlightcontroller(input_);
    }
    
    void ConnectedClientHandler::setInputValuesForDisconnect(const structs::config::Config &config) 
    {
        input_ = structs::middleware::Input();
        input_.throttle = config.logic.motors_off_disconnect ? 0 : config.controls.escs.idle;
        input_.joystick.degrees = 0;
        input_.joystick.offset = 0;
        input_.joystick.rotation = 0;
    }

    void ConnectedClientHandler::handleConnectedClient(std::unique_ptr<middleware::Client> &clientMw, std::unique_ptr<middleware::Flightcontroller> &fcMw, const structs::config::Config &config)
    {
        while(clientMw->clientConnectionAvailable()) 
        {
            if(clientMw->clientMessagesAvailable()) 
            {
                handleClientMessage(clientMw, fcMw);
            }
            if(fcMw->bytesOfAvailableData() > 0) 
            {
                auto successfulSend = handleFlightcontrollerMessage(clientMw, fcMw);
                if(!successfulSend) break;
            }
            fcMw->clearReceiverSerialBuffer();
        }
        LOG(INFO) << "Connection lost...";
        handleClientDisconnect(fcMw, config);
    }
}