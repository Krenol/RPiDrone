#ifndef DRONE_LOOP_H
#define DRONE_LOOP_H

#include "websocket.hpp"
#include <memory>
#include <string>
#include "flightcontroller.hpp"
#include "parsers/json/client_input_parser.hpp"
#include "structs/config/config.hpp"
#include "structs/flightcontroller/input.hpp"

using json = nlohmann::json;

namespace drone {
    class Loop  {
        public:
            Loop();

            void loop(Websocket &websocket, drone::Flightcontroller &fc, const structs::config::Config &config);
            
        private:
            ClientInput last_input_;

            void createOutputJson(float roll_is, float pitch_is, float yaw_is, float roll_should, float pitch_should, float yaw_should,const GPSCoordinates &c, json &j);

            void parseAppJson(std::string &msg, FlightcontrollerInput &userInput, const structs::config::Config &config);

            void sendToFlightcontroller(drone::Flightcontroller &fc, std::string &msg, FlightcontrollerInput &userInput, const structs::config::Config &config);
    };
}

#endif