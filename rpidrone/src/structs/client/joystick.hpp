#ifndef DRONE_STRUCT_JOYSTICK_H
#define DRONE_STRUCT_JOYSTICK_H

struct Joystick {
    float degrees = 0, offset = 0, rotation = 0;

    Joystick() {

    }

    Joystick(float degrees, float offset, float rotation) {
        this->degrees = degrees;
        this->offset = offset;
        this->rotation = rotation;
    }

    Joystick(const Joystick& j) {
        this->degrees = j.degrees;
        this->offset = j.offset;
        this->rotation = j.rotation;
    }
};

#endif