#ifndef DRONE_JSON_CONFIG_PARSER_H
#define DRONE_JSON_CONFIG_PARSER_H

#include <nlohmann/json.hpp>
#include <string>
#include "../../structs/config/config.hpp"

using json = nlohmann::json;

namespace drone
{
    namespace parsers {
        namespace json {
            class ConfigParser {
                private:
                    static void parseSensorObject(const nlohmann::json &j, structs::config::Config &cfg);
                    static void parseControlObject(const nlohmann::json &j, structs::config::Config &cfg);

                public:
                    static void parseJsonConfigToStruct(const nlohmann::json &j, structs::config::Config &cfg);
            };
        }
    }
} // namespace drone

#endif