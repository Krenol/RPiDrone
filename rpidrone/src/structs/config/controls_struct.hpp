#ifndef DRONE_STRUCT_CONFIG_CONTROLS_STRUCT_H
#define DRONE_STRUCT_CONFIG_CONTROLS_STRUCT_H

#include "escs.hpp"
namespace drone
{
    namespace structs
    {
        namespace config
        {
            struct ControlsStruct
            {
                float maxPitchRate = 20.0f, maxRollRate = 20.0f, maxYawVel = 20.0f;
                Escs escs;

                ControlsStruct()
                {
                }

                ControlsStruct(const Escs &esc, float maxPitchRate, float maxRollRate, float maxYawVel) : escs(esc)
                {
                    this->maxPitchRate = maxPitchRate;
                    this->maxRollRate = maxRollRate;
                    this->maxYawVel = maxYawVel;
                }

                ControlsStruct(const ControlsStruct &c) : escs(c.escs)
                {
                    this->maxPitchRate = c.maxPitchRate;
                    this->maxRollRate = c.maxRollRate;
                    this->maxYawVel = c.maxYawVel;
                }
            };
        }
    }
}

#endif