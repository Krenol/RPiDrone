#ifndef DRONE_PARSERS_FLIGHTCONTROLLER__PARSER_H
#define DRONE_PARSERS_FLIGHTCONTROLLER__PARSER_H

#include <string>
#include "../../structs/flightcontroller/output.hpp"

#define DELIM_ITEM ";"

namespace drone
{
    void parseOutputFromFlightcontroller(std::string &out, FlightcontrollerOutput &output);

} // namespace drone

#endif