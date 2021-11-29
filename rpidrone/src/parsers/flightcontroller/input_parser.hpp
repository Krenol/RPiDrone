#ifndef DRONE_PARSERS_FLIGHTCONTROLLER_INPUT_PARSER_H
#define DRONE_PARSERS_FLIGHTCONTROLLER_INPUT_PARSER_H

#include <string>
#include "../../structs/flightcontroller/input.hpp"

#define INPUT_TOKEN "<S>"
#define EOL "\n"
#define DELIM_ITEM ";"

namespace drone
{
    void parseControlInputForFlightcontroller(const FlightcontrollerInput &input, std::string &out);

} // namespace drone

#endif