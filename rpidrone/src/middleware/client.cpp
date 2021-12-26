#include "client.hpp"

namespace drone
{
    namespace middleware
    {
        void Client::parseOutputJson(const structs::middleware::Output &outMsg) 
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
        
        bool Client::parseInput(const structs::middleware::Input &inMsg) 
        {
            bool successfulParse = parseClientInpuStringToJson();
            if(successfulParse) {
                parsers::json::InputParser::parseInputJsonToStruct(inputJson_, inMsg);
            }
            return successfulParse;
        }
        
        bool Client::parseClientInpuStringToJson() 
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

        bool Client::sendToClient(const structs::middleware::Output &outMsg) 
        {
            parseOutputJson(outMsg);
            auto success = websocket_->writeMessage(outputJson_.dump());
            return success.get();
        }
        
        bool Client::receiveFromClient(structs::middleware::Input &clientInputMsg) 
        {
            if(clientMessagesAvailable()) {
                websocket_->getMessage(inputStringMsg_);
                return parseInput(clientInputMsg);
            } 
            return false;
        }
        
        bool Client::clientConnectionAvailable() 
        {
            return websocket_->hasConnection();
        }
        
        bool Client::clientMessagesAvailable() 
        {
            return websocket_->hasMessages();
        }
    }
}