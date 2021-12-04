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
                    static void appendValueDelim(std::string &out);
                    static void appendItemDelim(std::string &out);
                    static void appendEOL(std::string &out);
                    static void appendNewCategoryToken(std::string &out);
                    static void appendControllerConfigs(const structs::config::Config &config, std::string &out);
                    static void appendRollControllerConfigs(const structs::config::Config &config, std::string &out);
                    static void appendRollRateControllerConfigs(const structs::config::Config &config, std::string &out);
                    static void appendRollOutControllerConfigs(const structs::config::Config &config, std::string &out);
                    static void appendPitchControllerConfigs(const structs::config::Config &config, std::string &out);
                    static void appendPitchRateControllerConfigs(const structs::config::Config &config, std::string &out);
                    static void appendPitchOutControllerConfigs(const structs::config::Config &config, std::string &out);
                    static void appendYawControllerConfigs(const structs::config::Config &config, std::string &out);
                    static void appendEscConfigs(const structs::config::Config &config, std::string &out);
                public: 
                    static void parseFlightcontrollerConfig(const structs::config::Config &config, std::string &out);
            };
        }
    }
} // namespace drone

#endif