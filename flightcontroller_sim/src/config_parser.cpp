#include "config_parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool ConfigParser::parse(Config *c, char *msg, const char *delim1, const char *delim2) 
{
    //msg layout: <C>PID_DATA&ESC_DATA\n
    // e.g.: <C>1,1,1,1,1,1;2,2,2,2,2,2;3,3,3,3,3,3;4,4,4,4,4,4;5,5,5,5,5,5&700;2100;6;7;8;9;0\n
    if(strncmp(CONFIG_TOKEN, msg, 3) != 0) return false;
    msg = msg + 3;
    int i;
    char *str, *sptr; 
    for (i = 0; i < 2; msg = NULL)
    {
        str = strtok_r(msg, delim1, &sptr);
        if (str == NULL)
            return false;
        if(i == 0) {
            parsePID(c, str, delim2);
        } else if(i == 1) {
            parseESC(c, str, delim2);
        } else {
            break;
        }
        ++i;
    }
    return true;
}

void ConfigParser::parsePID(Config *c, char *msg, const char *delim) 
{
    int i;
    char *str, *sptr; 
    const char delim_inner = ',';
    float pidline[6]; //kp,kd,ki,kaw,min,max
    //PID SETUP: RV;RT;PV;PT;YT
    for (i = 0; i < 5; msg = NULL)
    {   
        str = strtok_r(msg, delim, &sptr);
        parsePIDLine(str, pidline, &delim_inner);
        if(i == 0) {
            c->kp_r_v = pidline[0];
            c->kd_r_v = pidline[1];
            c->ki_r_v = pidline[2];
            c->kaw_r_v = pidline[3];
            c->min_r_v = pidline[4];
            c->max_r_v = pidline[5];
        } else if(i == 1) {
            c->kp_r_t = pidline[0];
            c->kd_r_t = pidline[1];
            c->ki_r_t = pidline[2];
            c->kaw_r_t = pidline[3];
            c->min_r_t = pidline[4];
            c->max_r_t = pidline[5];
        } else if(i == 2) {
            c->kp_p_v = pidline[0];
            c->kd_p_v = pidline[1];
            c->ki_p_v = pidline[2];
            c->kaw_p_v = pidline[3];
            c->min_p_v = pidline[4];
            c->max_p_v = pidline[5];
        } else if(i == 3) {
            c->kp_p_t = pidline[0];
            c->kd_p_t = pidline[1];
            c->ki_p_t = pidline[2];
            c->kaw_p_t = pidline[3];
            c->min_p_t = pidline[4];
            c->max_p_t = pidline[5];
        } else if(i == 4) {
            c->kp_y_t = pidline[0];
            c->kd_y_t = pidline[1];
            c->ki_y_t = pidline[2];
            c->kaw_y_t = pidline[3];
            c->min_y_t = pidline[4];
            c->max_y_t = pidline[5];
        }
        ++i;
    }
}

void ConfigParser::parseESC(Config *c, char *msg, const char *delim) 
{
    int i;
    char *str, *sptr; 
    //ESC SETUP: MIN;MAX;LB;LF;RB;RF;CALIB
    for (i = 0; i < 7; msg = NULL)
    {   
        str = strtok_r(msg, delim, &sptr);
        if(i == 0) {
            c->esc_min = atoi(str);
        } else if(i == 1) {
            c->esc_max = atoi(str);
        } else if(i == 2) {
            c->pin_lb =atoi(str);
        } else if(i == 3) {
            c->pin_lf = atoi(str);
        } else if(i == 4) {
            c->pin_rb = atoi(str);
        } else if(i == 5) {
            c->pin_rf = atoi(str);
        } else if(i == 6) {
            c->calib_esc = atoi(str) == 1;
        }
        ++i;
    }
}

void ConfigParser::parsePIDLine(char *line, float *vals, const char *delim) 
{
    int i;
    char *str, *sptr; 
    //SETUP: RV:kp,kd,ki,kaw,min,max
    for (i = 0; i < 6; line = NULL)
    {   
        str = strtok_r(line, delim, &sptr);
        vals[i] = atof(str);
        ++i;
    }
}
