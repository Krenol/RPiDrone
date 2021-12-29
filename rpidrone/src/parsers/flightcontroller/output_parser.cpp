#include "output_parser.hpp"
#include <stdlib.h>
#include "constants/flightcontroller.hpp"

namespace drone
{
    namespace parsers
    {
        namespace flightcontroller
        {
            float OutputParser::convertStringToFloat(const std::string &value) 
            {
                return atof(value.c_str());
            }
            
            int OutputParser::convertStringToInt(const std::string &value) 
            {
                return atoi(value.c_str());
            }
        
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
                        output.isAngles.yaw = convertStringToFloat(token);
                    }
                    else if (i == 1)
                    {
                        output.isAngles.pitch = convertStringToFloat(token);
                    }
                    else if (i == 2)
                    {
                        output.isAngles.roll = convertStringToFloat(token);
                    }
                    else if (i == 3)
                    {
                        //TODO
                        //output.exec_time = convertStringToInt(token);
                    }
                    else if (i == 4)
                    {
                        output.acceleration.ax = convertStringToFloat(token);
                    }
                    else if (i == 5)
                    {
                        output.acceleration.ay = convertStringToFloat(token);
                    }
                    else if (i == 6)
                    {
                        output.acceleration.az = convertStringToFloat(token);
                    }
                    else if (i == 7)
                    {
                        //TODO
                        //output.rf_t = convertStringToInt(token);
                    }
                    else if (i == 8)
                    {
                        //TODO
                        //output.rb_t = convertStringToInt(token);
                    }
                    else if (i == 9)
                    {
                        //TODO
                        //output.lf_t = convertStringToInt(token);
                    }
                    else if (i == 10)
                    {
                        //TODO
                        //output.lb_t = convertStringToInt(token);
                    }
                    else if (i == 11)
                    {
                        output.throttle = convertStringToInt(token);
                    }
                    else if (i == 12)
                    {
                        output.shouldAngles.yaw = convertStringToFloat(token);
                    }
                    else if (i == 13)
                    {
                        output.shouldAngles.roll = convertStringToFloat(token);
                    }
                    else if (i == 14)
                    {
                        output.shouldAngles.pitch = convertStringToFloat(token);
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