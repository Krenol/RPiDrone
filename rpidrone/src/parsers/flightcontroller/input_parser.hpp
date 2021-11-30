#ifndef DRONE_PARSERS_FLIGHTCONTROLLER_INPUT_PARSER_H
#define DRONE_PARSERS_FLIGHTCONTROLLER_INPUT_PARSER_H

#include <string>
#include "../../structs/flightcontroller/input.hpp"

#define INPUT_TOKEN "<S>"
#define EOL "\n"
#define DELIM_ITEM ";"

namespace drone
{
    namespace parsers {
        namespace flightcontroller {
            class InputParser {
                private:
                    static void setInputToken(std::string &out);
                    static void appendDelim(std::string &out);
                    static void appendEOL(std::string &out);
                    static void appendYPRAngles(const FlightcontrollerInput &input, std::string &out);
                    static void appendThrottleValue(const FlightcontrollerInput &input, std::string &out);
                public: 
                    static void parseFlightcontrollerInput(const FlightcontrollerInput &input, std::string &out);
            };
        }
    }
} // namespace drone

#endif