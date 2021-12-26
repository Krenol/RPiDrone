#include "arduino.hpp"
#include "wiringSerial.h"
#include "logs/easylogging++.h"
#include "parsers/flightcontroller/config_parser.hpp"
#include "constants/flightcontroller.hpp"

namespace drone
{
    Arduino::Arduino(const std::string &serial, int baudrate) : fd_ard_{serialOpen(serial.c_str(), baudrate)}
    {
        if(fd_ard_ < 0) {
            throw std::bad_alloc();
        }
    }

    void Arduino::serialSend(const std::string &msg) 
    {
        serialPuts(fd_ard_, msg.c_str());
    }
    
    void Arduino::serialRead(char *msg, const char eol) 
    {
        serialGetStr(fd_ard_, msg, constants::flightcontroller::OUT_MSG_SIZE, eol);
    }

    int Arduino::availableData() 
    {
        return serialDataAvail(fd_ard_);
    }
    
    void Arduino::clearReceiverBuffer() 
    {
        clearReceiveBuffer(fd_ard_);
    }
    
    void Arduino::serialFlushBuffer() 
    {
        serialFlush(fd_ard_);
    }
}