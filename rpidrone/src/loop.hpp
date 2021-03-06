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
            Input last_input_;
            std::unique_ptr<rpisocket::WiFiServer> server_;
            int fd_ard_;
            
            std::unique_ptr<drone::Arduino> fc_;

            /**
             * Method to load the config file
             * @param file The file to be parsed
             */
            void loadConfig(const std::string& file);

            /**
             * Loop maintianing the connection to clients
             */
            void connectionHandler();

            void createOutputJson(float roll, float pitch, float yaw, const GPSCoordinates& c, json& j);

            void parseUserInput(std::string &msg, UserInput &userInput);

            void readFromSocket(std::string &buf, int max_iter = 10);

            bool parseBuffer(std::string &buf, std::string &msg);

            void sendToFlightcontroller(std::string &msg, UserInput &userInput);
    };
}

#endif