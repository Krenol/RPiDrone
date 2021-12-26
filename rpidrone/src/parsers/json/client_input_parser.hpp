#ifndef DRONE_JSON_INPUT_PARSER_H
#define DRONE_JSON_INPUT_PARSER_H

#include <nlohmann/json.hpp>
#include <string>
#include "../../structs/client/input.hpp"

using json = nlohmann::json;

namespace drone
{
    namespace parsers
    {
        namespace json
        {
            class InputParser
            {
            private:
                static void parseJoystickStruct(const nlohmann::json &json, Input &clientInput);
                static void parseThrottleStruct(const nlohmann::json &json, Input &clientInput);
                static void parseGpsStruct(const nlohmann::json &json, Input &clientInput);

            public:
                static void parseInputJsonToStruct(const nlohmann::json &json, Input &clientInput);
            };
        }
    }
} // namespace drone

#endif