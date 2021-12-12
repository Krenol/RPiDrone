#include "config.hpp"
#include <fstream>
#include "parsers/flightcontroller/config_parser.hpp"

namespace drone
{
    void Config::loadConfig(const std::string &conf_file, drone::structs::config::Config &config) 
    {
        std::ifstream ifs(conf_file);
        auto j = json::parse(ifs);
        drone::parsers::json::ConfigParser::parseJsonConfigToStruct(j, config);
    }
}