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
                output.yaw_is = atof(token.c_str());
            } else if(i == 1) {
                output.pitch_is = atof(token.c_str());
            } else if(i == 2) {
                output.roll_is = atof(token.c_str());
            } else if(i == 3) {
                output.exec_time = atoi(token.c_str());
            } else if(i == 4) {
                output.ax = atof(token.c_str());
            } else if(i == 5) {
                output.ay = atof(token.c_str());
            } else if(i == 6) {
                output.az = atof(token.c_str());
            } else if(i == 7) {
                output.rf_t = atoi(token.c_str());
            } else if(i == 8) {
                output.rb_t = atoi(token.c_str());
            } else if(i == 9) {
                output.lf_t = atoi(token.c_str());
            } else if(i == 10) {
                output.lb_t = atoi(token.c_str());
            }  else if(i == 11) {
                output.yaw_should = atoi(token.c_str());
            }  else if(i == 12) {
                output.pitch_should = atoi(token.c_str());
            }  else if(i == 13) {
                output.roll_should = atoi(token.c_str());
            } else {
                break;
            }
            out.erase(0, pos + std::string(DELIM_ITEM).length());
            ++i;
        }
    }
}