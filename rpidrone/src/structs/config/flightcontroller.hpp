#ifndef DRONE_STRUCT_CONFIG_FC_H
#define DRONE_STRUCT_CONFIG_FC_H

#include <string>

struct Flightcontroller {
    std::string port = "/dev/ttyACM0";
    int baudrate = 115200, max_serial_buffer = 200;

    Flightcontroller() {

    }

    Flightcontroller(const std::string &port, int baudrate, int max_serial_buffer) 
    {
        this->port = port;
        this->baudrate = baudrate;
        this->max_serial_buffer = max_serial_buffer;
    }

    Flightcontroller(const Flightcontroller &fc) 
    {
        this->port = fc.port;
        this->baudrate = fc.baudrate;
        this->max_serial_buffer = fc.max_serial_buffer;
    }
};


#endif