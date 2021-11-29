#ifndef DRONE_PARSERS_FLIGHTCONTROLLER_CONFIG_PARSER_H
#define DRONE_PARSERS_FLIGHTCONTROLLER_CONFIG_PARSER_H

#include <string>
#include "../json/config_parser.hpp"

#define CONFIG_TOKEN "<C>"
#define EOL "\n"
#define DELIM_CAT "&"
#define DELIM_ITEM ";"
#define DELIM_VAL ","

namespace drone
{
    namespace parsers {
        namespace flightcontroller {
            class ConfigParser {
                private:
                    static void setConfigToken(std::string &out);
                    static void appendNewCategoryToken(std::string &out);
                    static void appendControllerConfigs(const Config &config, std::string &out);
                    static void appendRollControllerConfigs(const Config &config, std::string &out);
                    static void appendPitchControllerConfigs(const Config &config, std::string &out);
                    static void appendYawControllerConfigs(const Config &config, std::string &out);
                    static void appendEscConfigs(const Config &config, std::string &out);
                public: 
                    static void parseFlightcontrollerConfig(const Config &config, std::string &out);
            };
        }
    }
} // namespace drone

#endif