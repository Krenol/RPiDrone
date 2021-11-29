#ifndef DRONE_STRUCT_CONFIG_CONFIG_H
#define DRONE_STRUCT_CONFIG_CONFIG_H

#include "logic.hpp"
#include "server.hpp"
#include "led.hpp"
#include "queues.hpp"
#include "sensors_struct.hpp"
#include "controls_struct.hpp"
#include "flightcontroller.hpp"

namespace drone {
    struct Config
    {
        Logic logic;
        Server server;
        Leds leds;
        Queues queues;
        SensorsStruct sensors;
        ControlsStruct controls;
        Flightcontroller flightcontroller;

        Config()
        {
        }

        Config(const Logic &logic, const Server &server, const Leds &leds, const Queues &queues, const SensorsStruct &sensors, const ControlsStruct &controls, const Flightcontroller &flightcontroller) : logic(logic), server(server), leds(leds), queues(queues), sensors(sensors), controls(controls), flightcontroller(flightcontroller)
        {
        }

        Config(const Config &c) : logic(c.logic), server(c.server), leds(c.leds), queues(c.queues), sensors(c.sensors), controls(c.controls), flightcontroller(c.flightcontroller)
        {
        }
    };
}

#endif