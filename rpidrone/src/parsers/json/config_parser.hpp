#ifndef DRONE_JSON_CONFIG_PARSER_H
#define DRONE_JSON_CONFIG_PARSER_H

#include <nlohmann/json.hpp>
#include <string>
#include "../../structs/config/config.hpp"

using json = nlohmann::json;

namespace drone
{
    void from_json(const nlohmann::json &j, Config &cfg);

    void parse_sensor_obj(const nlohmann::json &j, Config &cfg);

    void parse_control_obj(const nlohmann::json &j, Config &cfg);

} // namespace drone

#endif