#include "json_parser.hpp"
#include "rpicomponents/rpicomponents.hpp"
#include "globals.hpp"
#include <cmath>

#ifndef DRONE_INPUT_PARSER_H
#define DRONE_INPUT_PARSER_H


namespace drone
{
    struct ControlValues {
        int throttle;
        float roll_angle, pitch_angle, yaw_vel,
        rpicomponents::GPSCoordinates gps;

        ControlValues() {

        }

        ControlValues(const Input& input, const ControlsStruct &controls) 
        {
            this->throttle = input.throttle * (controls.escs.max - controls.escs.idle) / drone::THROTTLE_UPPER_BOUND + controls.escs.idle;
            roll_angle = input.joystick.offset * cos(input.joystick.degrees * M_PI / 180.0) * controls.max_roll;
            pitch_angle = input.joystick.offset * sin(input.joystick.degrees * M_PI / 180.0) * controls.max_pitch;
            yaw_vel = controls_.max_yawn * input.joystick.rotation / drone::ROTATION_UPPER_BOUND;
            gps = rpicomponents::GPSCoordinates(input.gps);
        }

        ControlValues(const ControlValues& i)
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