#ifndef DRONE_STRUCT_INPUT_H
#define DRONE_STRUCT_INPUT_H

#include "parsers/json_input_parser.hpp"
#include "parsers/json_config_parser.hpp"
#include "gps_coordinates.hpp"
#include "globals.hpp"
#include <cmath>


namespace drone
{
    struct UserInput {
        int throttle = 0;
        float roll_angle = 0.0f, pitch_angle = 0.0f, yaw_vel = 0.0f;
        GPSCoordinates gps;

        UserInput() {

        }

        UserInput(int throttle, float offset, float degrees, float max_roll, float max_pitch, float max_yawn, float rotation, GPSCoordinates &gps) 
        {
            this->throttle = throttle;
            roll_angle = offset * cos(degrees * M_PI / 180.0) * max_roll;
            pitch_angle = offset * sin(degrees * M_PI / 180.0) * max_pitch;
            yaw_vel = max_yawn * rotation / drone::ROTATION_UPPER_BOUND;
            gps = GPSCoordinates(gps);
        }

        UserInput(const UserInput& i)
        {
            this->throttle = i.throttle;
            this->roll_angle = i.roll_angle;
            this->pitch_angle = i.pitch_angle;
            this->yaw_vel = i.yaw_vel;
            this->gps = i.gps;
        }
    };
}

#endif