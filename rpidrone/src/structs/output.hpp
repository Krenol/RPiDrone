#ifndef DRONE_STRUCT_OUTPUT_H
#define DRONE_STRUCT_OUTPUT_H

struct Output {
    float yaw = 0, pitch = 0, roll = 0, ax = 0, ay = 0, az = 0;
    int exec_time = 0, rf_t = 0, rb_t = 0, lf_t = 0, lb_t = 0;
};

#endif