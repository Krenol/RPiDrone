#include <nlohmann/json.hpp>
#include <string>
#include "../structs/gps_struct.hpp"

#ifndef DRONE_JSON_INPUT_PARSER_H
#define DRONE_JSON_INPUT_PARSER_H

using json = nlohmann::json;

namespace drone
{
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


    struct Input {
        int throttle = 0;
        Joystick joystick;
        GPSCoordinates gps;

        Input() {

        }

        Input(const Joystick& j, const GPSCoordinates& gc, int throttle) : joystick(j), gps(gc)
        {
            this->throttle = throttle;
        }

        Input(const Input& i) : joystick(i.joystick), gps(i.gps)
        {
            this->throttle = i.throttle;
        }
    };


    void from_json(const nlohmann::json &j, Input &i);


    static const float DEGREE_UPPER_BOUND = 360.0f, OFFSET_UPPER_BOUND = 1.0f, DEGREE_LOWER_BOUND = -360.0f, OFFSET_LOWER_BOUND = 0.0f, ROTATION_UPPER_BOUND = 100.0f, ROTATION_LOWER_BOUND = -100.0f;
    static const int THROTTLE_UPPER_BOUND = 100, THROTTLE_LOWER_BOUND = 0;

} // namespace drone

#endif