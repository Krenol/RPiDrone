#include "control_parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void ControlParser::parse(Controls *c, char *msg, const char delim)
{
    c->yaw_rate = 0.0f;
    c->pitch_angle = 0.0f;
    c->roll_angle = 0.0f;
    c->throttle = 0;
    for (;; msg = NULL)
    {
        str = strtok_r(msg, &delim, &sptr);

        if (str == NULL)
            break;
        switch (str[0])
        {
        case 'Y':
            c->yaw_rate = atof(str + 1);
            break;
        case 'P':
            c->pitch_angle = atof(str + 1);
            break;
        case 'R':
            c->roll_angle = atof(str + 1);
            break;
        case 'S':
            c->throttle = atoi(str + 1);
            break;
        default:
            break;
        }
    }
}
