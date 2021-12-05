#include "flightcontroller.hpp"
#include "wiringSerial.h"
#include "logs/easylogging++.h"
#include "parsers/flightcontroller/config_parser.hpp"

namespace drone
{
    Flightcontroller::Flightcontroller(const std::string &serial, int baudrate) : fd_ard_{serialOpen(serial.c_str(), baudrate)}
    {
        initialized_ = false;
        if(fd_ard_ < 0) {
            throw std::bad_alloc();
        }
    }

    void Flightcontroller::init(const drone::structs::config::Config &config) 
    {
        std::string flightcontrollerConfig;
        drone::parsers::flightcontroller::ConfigParser::parseFlightcontrollerConfig(config, flightcontrollerConfig);
        LOG(INFO) << "Setting up arduino with parsed config " << flightcontrollerConfig;
        configureFlightcontroller(flightcontrollerConfig);
        LOG(INFO) << "arduino acknowledged config; waiting for successful sensor & motor setup";
        awaitSuccessfulSetup();
        LOG(INFO) << "arduino setup is completed";
        initialized_ = true;
    }
    
    void Flightcontroller::serialSend(const std::string &msg) 
    {
        if(!initialized_) return;
        serialPuts(fd_ard_, msg.c_str());
    }
    
    void Flightcontroller::serialRead(char *msg, const char eol) 
    {
        if(!initialized_) return;
        serialGetStr(fd_ard_, msg, OUT_MSG_SIZE, eol);
    }

    int Flightcontroller::availableData() 
    {
        return serialDataAvail(fd_ard_);
    }
    
    void Flightcontroller::clearReceiverBuffer() 
    {
        clearReceiveBuffer(fd_ard_);
    }
    
    void Flightcontroller::configureFlightcontroller(const std::string &config) 
    {
        char out_c[OUT_MSG_SIZE];
        std::string out;        
        do {
            serialFlush(fd_ard_);
            serialPuts(fd_ard_, config.c_str());
            serialGetStr(fd_ard_, out_c, OUT_MSG_SIZE, '\n');
            out = out_c;
        } while(out.find(ACK_TOKEN) == std::string::npos);
    }
    
    void Flightcontroller::awaitSuccessfulSetup() 
    {
        char out_c[OUT_MSG_SIZE];
        std::string out;   
        do {
            serialGetStr(fd_ard_, out_c, OUT_MSG_SIZE, '\n');
            out = out_c;
        } while(out.find(CONTROL_TOKEN) == std::string::npos);
    }
}