#include "parsers/json_input_parser.hpp"
#include "parsers/json_config_parser.hpp"
#include "rpicomponents/rpicomponents.hpp"
#include "globals.hpp"
#include <cmath>

#ifndef DRONE_INPUT_PARSER_H
#define DRONE_INPUT_PARSER_H


namespace drone
{
    struct UserInput {
        int throttle = 0;
        float roll_angle = 0.0f, pitch_angle = 0.0f, yaw_vel = 0.0f;
        rpicomponents::GPSCoordinates gps;

        UserInput() {

        }

        UserInput(const Input& input, const ControlsStruct &controls) 
        {
            this->throttle = input.throttle * (controls.escs.max - controls.escs.idle) / drone::THROTTLE_UPPER_BOUND + controls.escs.idle;
            roll_angle = input.joystick.offset * cos(input.joystick.degrees * M_PI / 180.0) * controls.max_roll;
            pitch_angle = input.joystick.offset * sin(input.joystick.degrees * M_PI / 180.0) * controls.max_pitch;
            yaw_vel = controls.max_yawn * input.joystick.rotation / drone::ROTATION_UPPER_BOUND;
            gps = rpicomponents::GPSCoordinates(input.gps);
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