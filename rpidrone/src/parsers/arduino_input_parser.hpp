#ifndef DRONE_ARDUINO_INPUT_PARSER_H
#define DRONE_ARDUINO_INPUT_PARSER_H

#include <string>
#include "../structs/user_input.hpp"

#define INPUT_TOKEN "<S>"
#define EOL "\n"
#define DELIM_ITEM ";"

namespace drone
{
    void parse_input(const UserInput &input, std::string &out);

} // namespace drone

#endif