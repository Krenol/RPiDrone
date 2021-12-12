#ifndef DRONE_STRUCT_CLIENT_INPUT_H
#define DRONE_STRUCT_CLIENT_INPUT_H

#include "../gps_coordinates.hpp"
#include "joystick.hpp"

namespace drone
{
    namespace structs
    {
        namespace middleware
        {
            struct ClientInput
            {
                int throttle = 0;
                Joystick joystick;
                GPSCoordinates gps;

                ClientInput()
                {
                }

                ClientInput(const Joystick &j, const GPSCoordinates &gc, int throttle) : joystick(j), gps(gc)
                {
                    this->throttle = throttle;
                }

                ClientInput(const ClientInput &i) : joystick(i.joystick), gps(i.gps)
                {
                    this->throttle = i.throttle;
                }
            };
        }
    }
}
#endif