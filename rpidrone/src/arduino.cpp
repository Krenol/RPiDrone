#include "arduino.hpp"
#include "wiringSerial.h"
#include "logs/easylogging++.h"

namespace drone
{
    Arduino::Arduino(const std::string &serial, int baudrate, int resetPin) : fd_ard_{serialOpen(serial.c_str(), baudrate)}
    {
        resetPin_ = std::make_unique<pin::DigitalPin>(resetPin);
        resetPin_->OutputOn();
        initialized_ = false;
    }

    void Arduino::init(const std::string &conf) 
    {
        char out_c[OUT_MSG_SIZE];
        std::string out;
        reset();
        LOG(INFO) << "Setting up arduino with parsed config " << conf;
        do {
            serialFlush(fd_ard_);
            serialPuts(fd_ard_, conf.c_str());
            serialGetStr(fd_ard_, out_c, OUT_MSG_SIZE, '\n');
            out = out_c;
        } while(out.find("<A>") == std::string::npos);
        LOG(INFO) << "arduino acknowledged config; waiting for successful sensor & motor setup";
        
        //await go from arduino
        do {
            serialGetStr(fd_ard_, out_c, OUT_MSG_SIZE, '\n');
            out = out_c;
        } while(out.find(CONTROL_TOKEN) == std::string::npos);
        LOG(INFO) << "arduino setup is completed";
        initialized_ = true;
    }
    
    void Arduino::serialSend(const std::string &msg) 
    {
        if(!initialized_) return;
        serialPuts(fd_ard_, msg.c_str());
    }
    
    void Arduino::serialRead(char *msg, const char EOL) 
    {
        if(!initialized_) return;
        serialGetStr(fd_ard_, msg, OUT_MSG_SIZE, EOL);
    }
    
    void Arduino::reset() 
    {
        LOG(INFO) << "arduino is being resetted";
        resetPin_->OutputOff();
        sleep(2);
        resetPin_->OutputOn();
        sleep(3);
        LOG(INFO) << "arduino reset is complete"
    }
    
    int Arduino::dataAvailable() 
    {
        return serialDataAvail(fd_ard_);
    }
    
    void Arduino::clearReceiveBuffer() 
    {
        clearReceiveBuffer(fd_ard_);
    }
}