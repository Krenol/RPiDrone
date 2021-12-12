#ifndef DRONE_MISC_STRING_H
#define DRONE_MISC_STRING_H

#include <string>
#include <sstream>

namespace drone
{
    namespace misc
    {
        template <class T>
        class StringParser {
            public:
                static std::string roundToString(const T value, const int n = 1)
                {
                    std::ostringstream out;
                    out.precision(n);
                    out << std::fixed << value;
                    return out.str();
                }
        };
    }
}

#endif