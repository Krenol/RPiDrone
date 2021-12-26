#include "loop.hpp"
#include <unistd.h>
#include "logs/easylogging++.h"
#include "parsers/flightcontroller/input_parser.hpp"
#include "parsers/flightcontroller/output_parser.hpp"
#include "misc/json.hpp"
#include "logs/log_definition.hpp"

namespace drone
{
    Loop::Loop()
    {
    }

    void Loop::loop(middleware::Client &clientMw, middleware::Flightcontroller &fcMw, const structs::config::Config &config)
    {
        structs::middleware::Input clientInput;
        structs::middleware::Output clientOutput;
        int bytesAvailable;
        while(clientMw.clientConnectionAvailable()) 
        {
            if(clientMw.clientMessagesAvailable()) 
            {
                clientMw.receiveFromClient(clientInput);
                fcMw.sendToFlightcontroller(clientInput);
            }
            bytesAvailable = fcMw.bytesOfAvailableData();
            if(bytesAvailable > 0) {
                fcMw.receiveFromFlightcontroller(clientOutput);
                auto successfulSend = clientMw.sendToClient(clientOutput);
                if(!successfulSend) break;
                if(bytesAvailable > config.flightcontroller.max_serial_buffer) {
                    fcMw.clearReceiverBuffer();
                }
            }
        }
        LOG(INFO) << "Connection lost...";
        //TODO
        //fcMw.sendToFlightcontroller()
    }
}