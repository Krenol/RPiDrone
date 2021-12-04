#ifndef DRONE_STRUCT_CONFIG_CONTROLS_STRUCT_H
#define DRONE_STRUCT_CONFIG_CONTROLS_STRUCT_H

#include "escs.hpp"
#include "max_ypr_rates.hpp"

namespace drone
{
    namespace structs
    {
        namespace config
        {
            struct ControlsStruct
            {
                MaxYprRates maxYprRates;
                Escs escs;

                ControlsStruct()
                {
                }

                ControlsStruct(const Escs &esc, const MaxYprRates &maxYprRates) : escs(esc), maxYprRates(maxYprRates)
                {
     
                }

                ControlsStruct(const ControlsStruct &c) : escs(c.escs), maxYprRates(c.maxYprRates)
                {
      
                }
            };
        }
    }
}

#endif