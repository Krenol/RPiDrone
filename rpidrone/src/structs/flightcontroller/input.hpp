#ifndef DRONE_STRUCT_FC_INPUT_H
#define DRONE_STRUCT_FC_INPUT_H

#include "parsers/json/client_input_parser.hpp"
#include "parsers/json/config_parser.hpp"
#include "../gps_coordinates.hpp"
#include "globals.hpp"
#include <cmath>


namespace drone
{
    struct FlightcontrollerInput {
        int throttle = 0;
        float roll_angle = 0.0f, pitch_angle = 0.0f, yaw_vel = 0.0f;
        GPSCoordinates gps;

        FlightcontrollerInput() {

        }

        FlightcontrollerInput(int throttle, float offset, float degrees, float maxRollRate, float maxPitchRate, float maxYawVel, float rotation, GPSCoordinates &gps) 
        {
            this->throttle = throttle;
            roll_angle = offset * cos(degrees * M_PI / 180.0) * maxRollRate;
            pitch_angle = offset * sin(degrees * M_PI / 180.0) * maxPitchRate;
            yaw_vel = maxYawVel * rotation / drone::ROTATION_UPPER_BOUND;
            gps = GPSCoordinates(gps);
        }

        FlightcontrollerInput(const FlightcontrollerInput& i)
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