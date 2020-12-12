#include <nlohmann/json.hpp>

using json = nlohmann::json;

static bool JSON_EXISTS(const json& j, const std::string& key)
{
    return j.find(key) != j.end();
}