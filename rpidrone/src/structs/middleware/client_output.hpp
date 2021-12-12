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
            struct ClientOutput
            {
                YPR isAngles, shouldAngles;
                GPSCoordinates gps;

                ClientOutput()
                {
                }

                ClientOutput(const YPR &isAngles, const YPR &shouldAngles, const GPSCoordinates &gps) : gps{gps}, isAngles{isAngles}, shouldAngles{shouldAngles}
                {
                }

                ClientOutput(const ClientOutput &clientOutput) : gps{clientOutput.gps}, isAngles{clientOutput.isAngles}, shouldAngles{clientOutput.shouldAngles}
                {
                }
            };
        }
    }
}
#endif