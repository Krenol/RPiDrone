#ifndef DRONE_STRUCT_MIDDLEWARE_ACC_H
#define DRONE_STRUCT_MIDDLEWARE_ACC_H

struct Acceleration {
    float ax = 0, ay = 0, az = 0;

    Acceleration() {

    }

    Acceleration(float ax, float ay, float az) {
        this->ax = ax;
        this->ay = ay;
        this->az = az;
    }

    Acceleration(const Acceleration& ypr) {
        ax = ypr.ax;
        ay = ypr.ay;
        az = ypr.az;
    }
};

#endif