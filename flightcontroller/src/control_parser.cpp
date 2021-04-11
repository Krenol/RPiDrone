#include "control_parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void ControlParser::parse(float* ypr, int *throttle, char *msg, const char *delim) 
{
    //msg layout: <S>Y;P;R;T\n
    //example <S>12.1;15.21;12.12;20\n
    if(strncmp(CONTROL_TOKEN, msg, 3) != 0) return;
    msg = msg + 3;
    int i;
    for (i = 0; i < 3; msg = NULL)
    {
        str = strtok_r(msg, delim, &sptr);
        if (str == NULL)
            return;
        ypr[i] = atof(str);
        ++i;
    }
    msg = NULL;
    str = strtok_r(msg, delim, &sptr);
    if (str == NULL)
        return;
    *throttle = atoi(str);
}
