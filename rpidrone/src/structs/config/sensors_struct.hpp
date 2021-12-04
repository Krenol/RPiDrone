#ifndef DRONE_STRUCT_CONFIG_SENSORS_STRUCT_H
#define DRONE_STRUCT_CONFIG_SENSORS_STRUCT_H

#include "gps.hpp"

namespace drone
{
    namespace structs
    {
        namespace config
        {
            struct SensorsStruct
            {
                bool calibrate;
                int decimals = 1;
                GPS gps;

                SensorsStruct()
                {
                }

                SensorsStruct(bool calibrate, const GPS &g, int decimals) : gps(g)
                {
                    this->decimals = decimals;
                    this->calibrate = calibrate;
                }

                SensorsStruct(const SensorsStruct &s) : gps(s.gps)
                {
                    this->decimals = s.decimals;
                    this->calibrate = s.calibrate;
                }
            };
        }
    }
}
#endif