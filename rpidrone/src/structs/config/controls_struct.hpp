#ifndef DRONE_STRUCT_CONFIG_CONTROLS_STRUCT_H
#define DRONE_STRUCT_CONFIG_CONTROLS_STRUCT_H

#include "escs.hpp"

struct ControlsStruct
{
    float max_pitch = 20.0f, max_roll = 20.0f, max_yawn = 20.0f;
    Escs escs;

    ControlsStruct()
    {
    }

    ControlsStruct(const Escs &esc, float max_pitch, float max_roll, float max_yawn) : escs(esc)
    {
        this->max_pitch = max_pitch;
        this->max_roll = max_roll;
        this->max_yawn = max_yawn;
    }

    ControlsStruct(const ControlsStruct &c) : escs(c.escs)
    {
        this->max_pitch = c.max_pitch;
        this->max_roll = c.max_roll;
        this->max_yawn = c.max_yawn;
    }
};


#endif