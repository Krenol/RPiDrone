#ifndef DRONE_STRUCT_CONFIG_ESCS_H
#define DRONE_STRUCT_CONFIG_ESCS_H

#include "pid_control.hpp"

namespace drone
{
    namespace structs
    {
        namespace config
        {
            struct Escs
            {
                bool calibrate = false;
                int min = 700, max = 2000, idle = 800, pin_lf = 0, pin_rf = 1, pin_lb = 2, pin_rb = 3;
                PIDControl roll_rate, pitch_rate, roll_output, pitch_output, yaw_output;

                Escs()
                {
                }

                Escs(const PIDControl &roll_rate, const PIDControl &pitch_rate, const PIDControl &roll_output, const PIDControl &pitch_output, const PIDControl &yaw_output, int min, int max, int idle, int pin_lf, int pin_rf, int pin_lb, int pin_rb, bool calibrate) : roll_rate(roll_rate), pitch_rate(pitch_rate), roll_output(roll_output), pitch_output(pitch_output), yaw_output(yaw_output)
                {
                    this->calibrate = calibrate;
                    this->idle = idle;
                    this->max = max;
                    this->min = min;
                    this->pin_lb = pin_lb;
                    this->pin_rb = pin_rb;
                    this->pin_lf = pin_lf;
                    this->pin_rf = pin_rf;
                }

                Escs(const Escs &e) : roll_rate(e.roll_rate), pitch_rate(e.pitch_rate), roll_output(e.roll_output), pitch_output(e.pitch_output), yaw_output(e.yaw_output)
                {
                    this->calibrate = e.calibrate;
                    this->idle = e.idle;
                    this->max = e.max;
                    this->min = e.min;
                    this->pin_lb = e.pin_lb;
                    this->pin_rb = e.pin_rb;
                    this->pin_lf = e.pin_lf;
                    this->pin_rf = e.pin_rf;
                }
            };
        }
    }
}
#endif