#include "client.hpp"
namespace drone
{
    namespace middleware
    {
        void Client::parseClientOutput(const structs::middleware::ClientOutput &outMsg) 
        {
            outputJson_ = {
                {"sensors", {
                    {"barometric_height", 0}
                }},
                {"position", {
                    {"altitude", outMsg.gps.altitude},
                    {"latitude", outMsg.gps.latitude},
                    {"longitude", outMsg.gps.longitude}
                }},
                {"angles", {
                    {"is", {
                        {"yaw", outMsg.isAngles.yaw},
                        {"pitch", outMsg.isAngles.pitch},
                        {"roll", outMsg.isAngles.roll}
                    }},
                    {"should", {
                        {"yaw", outMsg.shouldAngles.yaw},
                        {"pitch", outMsg.shouldAngles.pitch},
                        {"roll", outMsg.shouldAngles.roll}
                    }}
                }}
            };
        }
        
        bool Client::parseClientInput(const structs::middleware::ClientOutput &outMsg) 
        {
            bool successfulParse = parseStringMessageToJson();
            if(successfulParse) {

            }
            return successfulParse;
        }
        
        bool Client::parseStringMessageToJson() 
        {
            bool successfulParse = false;
            try {
                inputJson_ = json::parse(inputStringMsg_);
                successfulParse = true;
            } catch(std::exception &ex) {
                successfulParse = false;
            }
            return successfulParse;
        }
    
        Client::Client(std::unique_ptr<Websocket> websocket) : websocket_{std::move(websocket)}
        {
            
        }

        bool Client::sendToClient(const structs::middleware::ClientOutput &outMsg) 
        {
            parseClientOutput(outMsg);
            auto success = websocket_->writeMessage(outputJson_.dump());
            return success.get();
        }
        
        bool Client::receiveFromClient(structs::middleware::ClientInput &inMsg) 
        {
            if(clientMessagesAvailable()) {
                websocket_->getMessage(inputStringMsg_);

            } 
            return false;
        }
        
        bool Client::clientConnectionAvailable() 
        {
            
        }
        
        bool Client::clientMessagesAvailable() 
        {
            return websocket_->hasMessages();
        }
    }
}