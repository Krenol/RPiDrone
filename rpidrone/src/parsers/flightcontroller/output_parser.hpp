#ifndef DRONE_FCOUTPUT_PARSER_H
#define DRONE_FCOUTPUT_PARSER_H

#include <string>
#include "../../structs/flightcontroller/output.hpp"

#define DELIM_ITEM ";"

namespace drone
{
    void parseOutputFromFlightcontroller(std::string &out, FlightcontrollerOutput &output);

} // namespace drone

#endif