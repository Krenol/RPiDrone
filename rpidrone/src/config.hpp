#ifndef DRONE_CONFIG_H
#define DRONE_CONFIG_H

#include <nlohmann/json.hpp>
#include <string>
#include "structs/config/config.hpp"

using json = nlohmann::json;

namespace drone
{
    class Config
    {
    public:
        static void loadConfig(const std::string &conf_file, drone::structs::config::Config &config);
    };

}

#endif