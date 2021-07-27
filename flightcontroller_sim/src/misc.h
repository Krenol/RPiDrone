#ifndef _MISC_H_
#define _MISC_H_

#include <Arduino.h>

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

#endif /* _MISC_H_ */