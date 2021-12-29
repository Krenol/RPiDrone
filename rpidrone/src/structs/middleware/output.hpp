#ifndef DRONE_STRUCT_MIDDLEWARE_CLIENT_OUTPUT_H
#define DRONE_STRUCT_MIDDLEWARE_CLIENT_OUTPUT_H

#include "../gps_coordinates.hpp"
#include "../ypr.hpp"
#include "acceleration.hpp"

namespace drone
{
    namespace structs
    {
        namespace middleware
        {
            struct Output
            {
                YPR isAngles, shouldAngles;
                GPSCoordinates gps;
                Acceleration acceleration;
                int throttle;

                Output()
                {
                }

                Output(const YPR &isAngles, const YPR &shouldAngles, const GPSCoordinates &gps, const Acceleration &acc, int throttle) : gps{gps}, isAngles{isAngles}, shouldAngles{shouldAngles}, acceleration{acc}
                {
                    this->throttle = throttle;
                }

                Output(const Output &output) : gps{output.gps}, isAngles{output.isAngles}, shouldAngles{output.shouldAngles}, acceleration{output.acceleration}
                {
                    throttle = output.throttle;
                }
            };
        }
    }
}
#endif