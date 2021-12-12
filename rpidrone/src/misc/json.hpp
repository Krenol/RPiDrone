#ifndef DRONE_MISC_JSON_H
#define DRONE_MISC_JSON_H

#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace drone
{
    namespace misc
    {
        class Json
        {
        public:
            static bool jsonFieldExists(const json &j, const std::string &key)
            {
                return j.find(key) != j.end();
            }
        };
    }
}

#endif