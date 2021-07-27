#ifndef DRONE_LOOP_H
#define DRONE_LOOP_H

#include "rpisocket/rpisocket.hpp"
#include "connection.hpp"
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
             * @param config_path The path to the config.json
             */
            Loop();

            /**
             * Method containing the main logic
             */
            void loop(rpisocket::WiFiServer &server, drone::Arduino &fc, const Config &config);
            
        private:
            //std::unique_ptr<rpicomponents::Led> on_led_, status_led_;
            Input last_input_;

            /**
             * Loop maintianing the connection to clients
             */
            void connectionHandler();

            void createOutputJson(float roll, float pitch, float yaw, const GPSCoordinates& c, json& j);

            void parseUserInput(std::string &msg, UserInput &userInput, const Config &config);

            void readFromSocket(rpisocket::WiFiServer &server, std::string &buf, const std::string &delim, int max_iter = 5);

            bool parseBuffer(std::string &buf, std::string &msg, const std::string &delim);

            void sendToFlightcontroller(drone::Arduino &fc, std::string &msg, UserInput &userInput, const Config &config);
    };
}

#endif