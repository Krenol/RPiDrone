#ifndef DRONE_ARDUINO_OUTPUT_PARSER_H
#define DRONE_ARDUINO_OUTPUT_PARSER_H

#include <string>
#include "../structs/flightcontroller_output.hpp"

#define DELIM_ITEM ";"

namespace drone
{
    void parse_output(std::string &out, FlightcontrollerOutput &output);

} // namespace drone

#endif