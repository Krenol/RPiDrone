#ifndef DRONE_JSON_INPUT_PARSER_H
#define DRONE_JSON_INPUT_PARSER_H

#include <nlohmann/json.hpp>
#include <string>
#include "../../structs/client/client_input.hpp"

using json = nlohmann::json;

namespace drone
{
    void from_json(const nlohmann::json &j, ClientInput &i);

    void parseJoystickStruct(const nlohmann::json &j, ClientInput &i);
    void parseThrottleStruct(const nlohmann::json &j, ClientInput &i);
    void parseGpsStruct(const nlohmann::json &j, ClientInput &i);

    static const float DEGREE_UPPER_BOUND = 360.0f, OFFSET_UPPER_BOUND = 1.0f, DEGREE_LOWER_BOUND = -360.0f, OFFSET_LOWER_BOUND = 0.0f, ROTATION_UPPER_BOUND = 100.0f, ROTATION_LOWER_BOUND = -100.0f;
    static const int THROTTLE_UPPER_BOUND = 100, THROTTLE_LOWER_BOUND = 0;

} // namespace drone

#endif