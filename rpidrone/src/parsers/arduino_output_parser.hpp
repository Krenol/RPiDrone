#ifndef DRONE_ARDUINO_INPUT_PARSER_H
#define DRONE_ARDUINO_INPUT_PARSER_H

#include <string>
#include "../structs/output.hpp"

#define DELIM_ITEM ";"

namespace drone
{
    void parse_output(std::string &out, Output &output);

} // namespace drone

#endif