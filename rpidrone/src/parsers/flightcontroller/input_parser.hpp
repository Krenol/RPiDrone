#ifndef DRONE_PARSERS_FLIGHTCONTROLLER_INPUT_PARSER_H
#define DRONE_PARSERS_FLIGHTCONTROLLER_INPUT_PARSER_H

#include <string>
#include "../../structs/middleware/input.hpp"
#include "../../structs/config/config.hpp"

namespace drone
{
    namespace parsers {
        namespace flightcontroller {
            class InputParser {
                private:
                    static void setInputToken(std::string &out);
                    static void appendDelim(std::string &out);
                    static void appendEOL(std::string &out);
                    static void appendYPRAngles(const structs::middleware::Input &input, std::string &out, const structs::config::Config &config);
                    static float calculateShouldRollAngle(const structs::middleware::Input &input, const structs::config::Config &config);
                    static float calculateShouldPitchAngle(const structs::middleware::Input &input, const structs::config::Config &config);
                    static float calculateShouldYawVelocity(const structs::middleware::Input &input, const structs::config::Config &config);
                    static void appendThrottleValue(const structs::middleware::Input &input, std::string &out);
                public: 
                    static void parseFlightcontrollerInput(const structs::middleware::Input &input, std::string &out, const structs::config::Config &config);
            };
        }
    }
} // namespace drone

#endif