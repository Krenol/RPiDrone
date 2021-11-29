#ifndef DRONE_FC_CONFIG_PARSER_H
#define DRONE_FC_CONFIG_PARSER_H

#include <string>
#include "../json/config_parser.hpp"

#define CONFIG_TOKEN "<C>"
#define EOL "\n"
#define DELIM_CAT "&"
#define DELIM_ITEM ";"
#define DELIM_VAL ","

namespace drone
{
    void parseConfigForFlightcontroller(const Config &config, std::string &out);

} // namespace drone

#endif