#ifndef DRONE_STRUCT_OUTPUT_H
#define DRONE_STRUCT_OUTPUT_H

struct Output {
    float yaw_is = 0, pitch_is = 0, roll_is = 0, ax = 0, ay = 0, az = 0, yaw_should = 0, pitch_should = 0, roll_should = 0;
    int exec_time = 0, rf_t = 0, rb_t = 0, lf_t = 0, lb_t = 0;
};

#endif