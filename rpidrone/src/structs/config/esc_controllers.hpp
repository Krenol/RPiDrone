#ifndef DRONE_STRUCT_CONFIG_ESC_CONTROLLERS_H
#define DRONE_STRUCT_CONFIG_ESC_CONTROLLERS_H

#include "pid.hpp"

namespace drone
{
    namespace structs
    {
        namespace config
        {
            struct EscControllers
            {
                PID roll_rate, pitch_rate, roll_output, pitch_output, yaw_output;

                EscControllers()
                {
                }

                EscControllers(const PID &roll_rate, const PID &pitch_rate, const PID &roll_output, const PID &pitch_output, const PID &yaw_output) : roll_rate(roll_rate), pitch_rate(pitch_rate), roll_output(roll_output), pitch_output(pitch_output), yaw_output(yaw_output)
                {
      
                }

                EscControllers(const EscControllers &e) : roll_rate(e.roll_rate), pitch_rate(e.pitch_rate), roll_output(e.roll_output), pitch_output(e.pitch_output), yaw_output(e.yaw_output)
                {

                }
            };
        }
    }
}
#endif