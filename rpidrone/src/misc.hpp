#ifndef DRONE_MISC_H
#define DRONE_MISC_H

#include <nlohmann/json.hpp>
#include <cmath>
#include <string>
#include <sstream>
#include <fstream>
#include <cstdio>
#include <iostream>
#include <ios>

using json = nlohmann::json;

static bool JSON_EXISTS(const json &j, const std::string &key)
{
    return j.find(key) != j.end();
}

template <typename T>
T ROUND(T number, int decimals)
{
    int fac = pow(10, decimals);
    return round(number * fac) / fac;
}

template <typename T>
T BOUND(T number, T lower, T upper)
{
    if (number > upper)
    {
        return upper;
    }
    else if (number < lower)
    {
        return lower;
    }
    return number;
}

template <typename T>
std::string TO_STRING(const T a_value, const int n = 1)
{
    std::ostringstream out;
    out.precision(n);
    out << std::fixed << a_value;
    return out.str();
}

#endif