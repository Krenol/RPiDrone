#ifndef DRONE_LOOP_H
#define DRONE_LOOP_H

#include "rpisocket/rpisocket.hpp"
#include "connection.hpp"
#include <memory>
#include <string>
#include "parsers/json_config_parser.hpp" 
#include "parsers/json_input_parser.hpp"
#include "structs/user_input.hpp"

#define OUT_MSG_SIZE 64
#define CONTROL_TOKEN "<S>"

using json = nlohmann::json;

namespace drone {
    class Loop  {
        public:
            /**
             * Constructor for loop class
             * @param config_path The path to the config.json
             */
            Loop(const std::string& config_path);

            /**
             * Destructor
             */
            ~Loop();

            /**
             * Method containing the main logic
             */
            void loop();
            
        private:
            //std::unique_ptr<rpicomponents::Led> on_led_, status_led_;
            Config config_;
            std::unique_ptr<Connection> connection_; 
            Input last_input_;
            int fd_ard_;

            /**
             * Method to load the config file
             * @param file The file to be parsed
             */
            void loadConfig(const std::string& file);

            /**
             * Loop maintianing the connection to clients
             */
            void connectionHandler();

            void createOutputJson(float roll, float pitch, const GPSCoordinates& c, json& j);

            void parseUserInput(std::string &msg, UserInput &userInput);

    };
}

#endif