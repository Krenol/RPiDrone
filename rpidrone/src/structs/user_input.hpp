#include "parsers/json_input_parser.hpp"
#include "parsers/json_config_parser.hpp"
#include "gps_struct.hpp"
#include "globals.hpp"
#include <cmath>

#ifndef DRONE_INPUT_PARSER_H
#define DRONE_INPUT_PARSER_H


namespace drone
{
    struct UserInput {
        int throttle = 0;
        float roll_angle = 0.0f, pitch_angle = 0.0f, yaw_vel = 0.0f;
        GPSCoordinates gps;

        UserInput() {

        }

        UserInput(int throttle, int idle, int max, float offset, float degrees, float max_roll, float max_pitch, float max_yawn, float rotation, GPSCoordinates &gps) 
        {
            this->throttle = throttle * (max - idle) / drone::THROTTLE_UPPER_BOUND + idle;
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