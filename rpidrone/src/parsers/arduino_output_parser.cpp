#include "arduino_output_parser.hpp"
#include <stdlib.h>

namespace drone
{
    void parse_output(std::string &out, Output &output)
    {
        size_t pos = 0;
        std::string token;
        int i = 0;
        while ((pos = out.find(DELIM_ITEM)) != std::string::npos)
        {
            token = out.substr(0, pos);
            if(i == 0) {
                output.yaw = atof(token.c_str());
            } else if(i == 1) {
                output.pitch = atof(token.c_str());
            } else if(i == 2) {
                output.roll = atof(token.c_str());
            } else if(i == 3) {
                output.height = atof(token.c_str());
            } else if(i == 4) {
                output.temperature = atof(token.c_str());
            } else {
                break;
            }
            out.erase(0, pos + std::string(DELIM_ITEM).length());
            ++i;
        }
    }
}