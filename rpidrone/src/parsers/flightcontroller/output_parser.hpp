#ifndef DRONE_PARSERS_FLIGHTCONTROLLER__PARSER_H
#define DRONE_PARSERS_FLIGHTCONTROLLER__PARSER_H

#include <string>
#include "../../structs/middleware/output.hpp"

namespace drone
{
    namespace parsers
    {
        namespace flightcontroller
        {
            class OutputParser
            {
            private:
                static float convertStringToFloat(const std::string &value);
                static int convertStringToInt(const std::string &value);

            public:
                static void parseOutputFromFlightcontroller(std::string &out, structs::middleware::Output &output);
            };
        }
    }
} // namespace drone

#endif