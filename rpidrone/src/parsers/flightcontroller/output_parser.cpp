#include "output_parser.hpp"
#include <stdlib.h>
#include "constants/flightcontroller.hpp"

namespace drone
{
    namespace parsers
    {
        namespace flightcontroller
        {
            void OutputParser::parseOutputFromFlightcontroller(std::string &out, structs::middleware::Output &output)
            {
                size_t pos = 0;
                std::string token;
                int i = 0;
                while ((pos = out.find(constants::flightcontroller::ITEM_DELIMITER)) != std::string::npos)
                {
                    token = out.substr(0, pos);
                    if (i == 0)
                    {
                        output.isAngles.yaw = atof(token.c_str());
                    }
                    else if (i == 1)
                    {
                        output.isAngles.pitch = atof(token.c_str());
                    }
                    else if (i == 2)
                    {
                        output.isAngles.roll = atof(token.c_str());
                    }
                    else if (i == 3)
                    {
                        //TODO
                        //output.exec_time = atoi(token.c_str());
                    }
                    else if (i == 4)
                    {
                        //TODO
                        //output.ax = atof(token.c_str());
                    }
                    else if (i == 5)
                    {
                        //TODO
                        //output.ay = atof(token.c_str());
                    }
                    else if (i == 6)
                    {
                        //TODO
                        //output.az = atof(token.c_str());
                    }
                    else if (i == 7)
                    {
                        //TODO
                        //output.rf_t = atoi(token.c_str());
                    }
                    else if (i == 8)
                    {
                        //TODO
                        //output.rb_t = atoi(token.c_str());
                    }
                    else if (i == 9)
                    {
                        //TODO
                        //output.lf_t = atoi(token.c_str());
                    }
                    else if (i == 10)
                    {
                        //TODO
                        //output.lb_t = atoi(token.c_str());
                    }
                    else if (i == 11)
                    {
                        //TODO
                        //output.throttle = atoi(token.c_str());
                    }
                    else if (i == 12)
                    {
                        output.shouldAngles.yaw = atof(token.c_str());
                    }
                    else if (i == 13)
                    {
                        output.shouldAngles.roll = atof(token.c_str());
                    }
                    else if (i == 14)
                    {
                        output.shouldAngles.pitch = atof(token.c_str());
                    }
                    else
                    {
                        break;
                    }
                    out.erase(0, pos + std::string(constants::flightcontroller::ITEM_DELIMITER).length());
                    ++i;
                }
            }
        }
    }
}