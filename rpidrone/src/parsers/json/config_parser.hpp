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
                    static nlohmann::json getLedsJsonObject(const nlohmann::json &configJson);
                    static nlohmann::json getLogicJsonObject(const nlohmann::json &configJson);
                    static nlohmann::json getServerJsonObject(const nlohmann::json &configJson);
                    static nlohmann::json getQueuesJsonObject(const nlohmann::json &configJson);
                    static nlohmann::json getFlightcontrollerJsonObject(const nlohmann::json &configJson);       
                    static nlohmann::json getControlsJsonObject(const nlohmann::json &configJson);
                    static nlohmann::json getSensorsJsonObject(const nlohmann::json &configJson);
                    static nlohmann::json getGpsJsonObject(const nlohmann::json &configJson);
                    static nlohmann::json getDroneControlsJsonObject(const nlohmann::json &configJson);
                    static nlohmann::json getEscsJsonObject(const nlohmann::json &configJson);
                    static nlohmann::json getEscPinsJsonObject(const nlohmann::json &configJson);
                    static nlohmann::json getControllersJsonObject(const nlohmann::json &configJson);
                    static void setSensorsStruct(const nlohmann::json &configJson, structs::config::Config &cfg);
                    static structs::config::GPS getGpsStruct(const nlohmann::json &configJson);
                    static void setControlsStruct(const nlohmann::json &configJson, structs::config::Config &cfg);
                    static structs::config::MaxYprRates getMaxYprRates(const nlohmann::json &configJson);
                    static structs::config::Escs getEscs(const nlohmann::json &configJson);
                    static structs::config::EscPins getEscPins(const nlohmann::json &configJson);
                    static structs::config::EscControllers getEscControllers(const nlohmann::json &configJson);
                    static structs::config::PID parsePidControlStruct(const nlohmann::json &controllersJson, const std::string &key);
                    static void setLedsStruct(const nlohmann::json &configJson, structs::config::Config &cfg);
                    static void setLogicStruct(const nlohmann::json &configJson, structs::config::Config &cfg);
                    static void setServerStruct(const nlohmann::json &configJson, structs::config::Config &cfg);
                    static void setQueuesStruct(const nlohmann::json &configJson, structs::config::Config &cfg);
                    static void setFlightcontrollerStruct(const nlohmann::json &configJson, structs::config::Config &cfg);
                public:
                    static void parseJsonConfigToStruct(const nlohmann::json &configJson, structs::config::Config &cfg);
            };
        }
    }
} // namespace drone

#endif