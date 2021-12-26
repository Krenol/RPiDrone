#include "flightcontroller.hpp"
#include "constants/flightcontroller.hpp"
#include "parsers/flightcontroller/input_parser.hpp"
#include "parsers/flightcontroller/output_parser.hpp"
#include "parsers/flightcontroller/config_parser.hpp"

using namespace std::chrono;

namespace drone
{
    namespace middleware
    {
        void Flightcontroller::configureFlightcontroller(const structs::config::Config &config) 
        {
            config_ = config;
            std::string configStr;
            parseConfigStructToString(config, configStr);
            sendConfigurationToFlightcontroller(configStr);
            awaitSuccessfulSetup();
        }

        void Flightcontroller::parseConfigStructToString(const structs::config::Config &config, std::string &configStr) 
        {
            parsers::flightcontroller::ConfigParser::parseFlightcontrollerConfig(config, configStr);
        }
        
        void Flightcontroller::sendConfigurationToFlightcontroller(const std::string &config) 
        {
            char out_c[constants::flightcontroller::OUT_MSG_SIZE];
            std::string out;
            auto startTime = steady_clock::now();
            do {
                checkForConfigurationTimeout(startTime);
                arduino_->serialFlushBuffer();
                arduino_->serialSend(config);
                arduino_->serialRead(out_c);
                out = out_c;
            } while(!tokenExistsInString(out, constants::flightcontroller::ACK_TOKEN));
        }
        
        void Flightcontroller::checkForConfigurationTimeout(const steady_clock::time_point &startTime) 
        {
            if(duration_cast<seconds>(steady_clock::now()-startTime).count() > constants::flightcontroller::SETUP_TIMEOUT_IN_SECONDS) {
                throw std::runtime_error("timeout occurred while setting up flightcontroller");
            }
        }
        
        void Flightcontroller::awaitSuccessfulSetup() 
        {
            char out_c[constants::flightcontroller::OUT_MSG_SIZE];
            std::string out;   
            auto startTime = steady_clock::now();
            do {
                checkForConfigurationTimeout(startTime);
                arduino_->serialRead(out_c);
                out = out_c;
            } while(!tokenExistsInString(out, constants::flightcontroller::CONTROL_TOKEN));
        }
        
        bool Flightcontroller::tokenExistsInString(const std::string &string, const std::string &token) 
        {
            return string.find(token) != std::string::npos;
        }
        
        Flightcontroller::Flightcontroller(std::unique_ptr<Arduino> arduino, const structs::config::Config &config) : arduino_{std::move(arduino)}
        {
            configureFlightcontroller(config);
        }

        void Flightcontroller::sendToFlightcontroller(const structs::middleware::Input &inMsg) 
        {
            std::string msg;
            parsers::flightcontroller::InputParser::parseFlightcontrollerInput(inMsg, msg, config_);
            arduino_->serialSend(msg);
        }
        
        void Flightcontroller::receiveFromFlightcontroller(structs::middleware::Output &outMsg) 
        {
            char cMsg[constants::flightcontroller::OUT_MSG_SIZE];
            arduino_->serialRead(cMsg, constants::flightcontroller::EOL);
            std::string msg = cMsg;
            parsers::flightcontroller::OutputParser::parseOutputFromFlightcontroller(msg, outMsg);
        }
        
        int Flightcontroller::bytesOfAvailableData() 
        {
            return arduino_->availableData();
        }
        
        void Flightcontroller::clearReceiverBuffer() 
        {
            arduino_->clearReceiverBuffer();
        }
    }
}