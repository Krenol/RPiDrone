#ifndef _MISC_H_
#define _MISC_H_

#include <Arduino.h>

float BOUND(float number, float lower, float upper)
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

#endif /* _MISC_H_ */