#ifndef DRONE_STRUCT_CONFIG_ESCS_H
#define DRONE_STRUCT_CONFIG_ESCS_H

#include "esc_controllers.hpp"
#include "esc_pins.hpp"

namespace drone
{
    namespace structs
    {
        namespace config
        {
            struct Escs
            {
                bool calibrate = false;
                EscPins esc_pins;
                int min = 700, max = 2000, idle = 800;
                EscControllers esc_controllers;

                Escs()
                {
                }

                Escs(const EscControllers &esc_controllers, EscPins esc_pins, int min, int max, int idle, bool calibrate) : esc_controllers(esc_controllers), esc_pins(esc_pins)
                {
                    this->calibrate = calibrate;
                    this->idle = idle;
                    this->max = max;
                    this->min = min;
                }

                Escs(const Escs &e) : esc_controllers(e.esc_controllers), esc_pins(e.esc_pins)
                {
                    calibrate = e.calibrate;
                    idle = e.idle;
                    max = e.max;
                    min = e.min;
                }
            };
        }
    }
}
#endif