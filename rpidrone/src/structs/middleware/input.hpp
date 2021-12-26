#ifndef DRONE_STRUCT_MIDDLEWARE_CLIENT_INPUT_H
#define DRONE_STRUCT_MIDDLEWARE_CLIENT_INPUT_H

#include "../gps_coordinates.hpp"
#include "joystick.hpp"

namespace drone
{
    namespace structs
    {
        namespace middleware
        {
            struct Input
            {
                int throttle = 0;
                Joystick joystick;
                GPSCoordinates gps;

                Input()
                {
                }

                Input(const Joystick &j, const GPSCoordinates &gc, int throttle) : joystick(j), gps(gc)
                {
                    this->throttle = throttle;
                }

                Input(const Input &clientInput) : joystick(clientInput.joystick), gps(clientInput.gps)
                {
                    this->throttle = clientInput.throttle;
                }
            };
        }
    }
}
#endif