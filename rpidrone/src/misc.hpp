#include <nlohmann/json.hpp>
#include <cmath>

using json = nlohmann::json;

static bool JSON_EXISTS(const json& j, const std::string& key)
{
    return j.find(key) != j.end();
}

template<typename T>
T ROUND(T number, int decimals)
{
    int fac = pow(10, decimals);
    return round(number * fac) / fac;
}