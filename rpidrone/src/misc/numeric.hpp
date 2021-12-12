#ifndef DRONE_MISC_NUMERIC_H
#define DRONE_MISC_NUMERIC_H

#include <cmath>

namespace drone
{
    namespace misc
    {
        template <class T>
        class Numeric
        {
        public:
            static T round(T number, int decimals)
            {
                int fac = pow(10, decimals);
                return round(number * fac) / fac;
            }

            static T bound(T number, T lower, T upper)
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
        };
    }
}

#endif