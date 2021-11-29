#ifndef DRONE_LOOP_H
#define DRONE_LOOP_H

#include "websocket.hpp"
#include <memory>
#include <string>
#include "arduino.hpp"
#include "parsers/json/client_input_parser.hpp"
#include "structs/config/config.hpp"
#include "structs/flightcontroller/input.hpp"

using json = nlohmann::json;

namespace drone {
    class Loop  {
        public:
            /**
             * Constructor for loop class
             */
            Loop();

            /**
             * Method containing the main logic
             */
            void loop(Websocket &websocket, drone::Arduino &fc, const Config &config);
            
        private:
            //std::unique_ptr<rpicomponents::Led> on_led_, status_led_;
            ClientInput last_input_;

            void createOutputJson(float roll_is, float pitch_is, float yaw_is, float roll_should, float pitch_should, float yaw_should,const GPSCoordinates &c, json &j);

            void parseAppJson(std::string &msg, FlightcontrollerInput &userInput, const Config &config);

            void sendToFlightcontroller(drone::Arduino &fc, std::string &msg, FlightcontrollerInput &userInput, const Config &config);
    };
}

#endif