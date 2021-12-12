#ifndef DRONE_STRUCT_YPR_H
#define DRONE_STRUCT_YPR_H

struct YPR {
    float yaw = 0, pitch = 0, roll = 0;

    YPR() {

    }

    YPR(float yaw, float pitch, float roll) {
        this->yaw = yaw;
        this->pitch = pitch;
        this->roll = roll;
    }

    YPR(const YPR& ypr) {
        yaw = ypr.yaw;
        pitch = ypr.pitch;
        roll = ypr.roll;
    }
};

#endif