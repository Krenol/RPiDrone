#include "client.hpp"
#include "parsers/json/client_input_parser.hpp"
#include "logs/log_definition.hpp"

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
        
        bool Client::parseInput(structs::middleware::Input &inMsg) 
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
            if(!websocket_->websocketThreadIsRunning()) {
                throw std::runtime_error("received inactive websocket");
            }
        }

        bool Client::sendToClient(const structs::middleware::Output &outMsg) 
        {
            std::promise<bool> promise;
            parseOutputJson(outMsg);
            auto msg = outputJson_.dump();
            NETWORK_LOG(INFO) << "Sending " << msg;
            websocket_->writeMessage(msg, promise);
            return promise.get_future().get();
        }
        
        bool Client::receiveFromClient(structs::middleware::Input &clientInputMsg) 
        {
            if(clientMessagesAvailable()) {
                websocket_->getMessage(inputStringMsg_);
                NETWORK_LOG(INFO) << "received " << inputStringMsg_;
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
        
        void Client::disconnectConnectedClient() 
        {
            websocket_->disconnectConnectedClient();
        }
    }
}