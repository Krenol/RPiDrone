#include "arduino.hpp"
#include "wiringSerial.h"
#include "logs/easylogging++.h"
#if defined(SIMULATOR_MODE)
#include <unistd.h>
#endif

namespace drone
{
    Arduino::Arduino(const std::string &serial, int baudrate) : fd_ard_{serialOpen(serial.c_str(), baudrate)}
    {
        initialized_ = false;
        if(fd_ard_ < 0) {
            throw std::bad_alloc();
        }
    }

    void Arduino::init(const std::string &conf) 
    {
        char out_c[OUT_MSG_SIZE];
        std::string out;
        #if defined(SIMULATOR_MODE)
        LOG(INFO) << "---SIMULATOR MODE ACTIVE -> RESET ARDUINO---";
        serialSend("<R>");
        LOG(INFO) << "---SIMULATOR MODE ACTIVE -> RESET SIGNAL SENT; WAIT 5s---";
        usleep(5000);
        #endif
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

    int Arduino::availableData() 
    {
        return serialDataAvail(fd_ard_);
    }
    
    void Arduino::clearReceiverBuffer() 
    {
        clearReceiveBuffer(fd_ard_);
    }
}