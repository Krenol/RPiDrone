#ifndef DRONE_LOOP_H
#define DRONE_LOOP_H

#include "websocket.hpp"
#include <memory>
#include <string>
#include "arduino.hpp"
#include "parsers/json_config_parser.hpp" 
#include "parsers/json_input_parser.hpp"
#include "structs/user_input.hpp"

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
            Input last_input_;

            void createOutputJson(float roll_is, float pitch_is, float yaw_is, float roll_should, float pitch_should, float yaw_should,const GPSCoordinates &c, json &j);

            void parseUserInput(std::string &msg, UserInput &userInput, const Config &config);

            void sendToFlightcontroller(drone::Arduino &fc, std::string &msg, UserInput &userInput, const Config &config);
    };
}

#endif