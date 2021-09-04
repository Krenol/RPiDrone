#include "out_parser.h"
#include <Arduino.h>

void OutParser::parse(char *msg, int msg_size, const char eol, const char sep, float yaw, float pitch, float roll, int exec, float ax, float ay, float az, int rf_t, int rb_t, int lf_t, int lb_t, int throttle, float *ypr_arr) {
    String(String(yaw) + sep + pitch + sep + roll + sep + exec + sep + ax + sep + ay + sep + az + sep + rf_t + sep + rb_t  + sep + lf_t  + sep + lb_t + sep + throttle + sep + ypr_arr[0] + sep + ypr_arr[1] + sep + ypr_arr[2] + eol).toCharArray(msg, msg_size);
}