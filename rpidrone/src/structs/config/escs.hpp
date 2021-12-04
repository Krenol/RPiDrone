#ifndef DRONE_STRUCT_CONFIG_ESCS_H
#define DRONE_STRUCT_CONFIG_ESCS_H

#include "esc_controllers.hpp"

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
                EscControllers esc_controllers;

                Escs()
                {
                }

                Escs(const EscControllers &esc_controllers, int min, int max, int idle, int pin_lf, int pin_rf, int pin_lb, int pin_rb, bool calibrate) : esc_controllers(esc_controllers)
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

                Escs(const Escs &e) : esc_controllers(e.esc_controllers)
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