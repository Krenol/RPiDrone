#include "arduino_input_parser.hpp"

namespace drone
{
    void parse_input(const UserInput &input, std::string &out) 
    {
        out = INPUT_TOKEN;
        out += std::to_string(input.yaw_vel) + DELIM_ITEM + std::to_string(input.pitch_angle) + DELIM_ITEM 
            + std::to_string(input.roll_angle) + DELIM_ITEM + std::to_string(input.throttle) + EOL;
    }
}