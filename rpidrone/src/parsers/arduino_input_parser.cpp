#include "arduino_input_parser.hpp"
#include "misc.hpp"

namespace drone
{
    void parse_input(const UserInput &input, std::string &out) 
    {
        out = INPUT_TOKEN;
        out += TO_STRING<float>(input.yaw_vel, 1) + DELIM_ITEM + TO_STRING<float>(input.pitch_angle, 1) + DELIM_ITEM 
            + TO_STRING<float>(input.roll_angle, 1) + DELIM_ITEM + std::to_string(input.throttle) + EOL;
    }
}