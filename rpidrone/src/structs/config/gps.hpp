#ifndef DRONE_STRUCT_CONFIG_GPS_H
#define DRONE_STRUCT_CONFIG_GPS_H

#include <string>

struct GPS
{
    std::string port = "/dev/serial0";
    int baudrate = 9600;

    GPS()
    {
    }

    GPS(const std::string &port, int baudrate)
    {
        this->port = port;
        this->baudrate = baudrate;
    }

    GPS(const GPS &g)
    {
        this->port = g.port;
        this->baudrate = g.baudrate;
    }
};


#endif