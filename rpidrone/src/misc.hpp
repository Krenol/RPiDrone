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

template<typename T>
T BOUND(T number, T lower, T upper)
{
    if(number > upper){
        return upper;
    } else if(number < lower){
        return lower;
    }
    return number;
}