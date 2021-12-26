#ifndef DRONE_STRUCT_MIDDLEWARE_CLIENT_OUTPUT_H
#define DRONE_STRUCT_MIDDLEWARE_CLIENT_OUTPUT_H

#include "../gps_coordinates.hpp"
#include "../ypr.hpp"

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

                Output()
                {
                }

                Output(const YPR &isAngles, const YPR &shouldAngles, const GPSCoordinates &gps) : gps{gps}, isAngles{isAngles}, shouldAngles{shouldAngles}
                {
                }

                Output(const Output &clientOutput) : gps{clientOutput.gps}, isAngles{clientOutput.isAngles}, shouldAngles{clientOutput.shouldAngles}
                {
                }
            };
        }
    }
}
#endif