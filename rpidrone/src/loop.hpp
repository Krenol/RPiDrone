#include "rpisocket/rpisocket.hpp"
#include "connection.hpp"
#include "controls.hpp"
#include <memory>
#include <string>
#include <atomic>
#include "parsers/json_config_parser.hpp"
#include "server_sub_q.hpp"
#include "sensors.hpp"
#include "rpicomponents/rpicomponents.hpp"

#ifndef DRONE_LOOP_H
#define DRONE_LOOP_H

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

            /**
             * Await button press and startup everything
             */
            void startupDrone();
            
        private:

            std::unique_ptr<rpicomponents::Led> on_led_, status_led_;
            std::unique_ptr<Sensors> sensors_;
            std::unique_ptr<Controls> controls_;
            Config config_;
            std::unique_ptr<Connection> connection_;
            std::shared_ptr<drone::ServerSubscriberQueue> server_q_;
            std::unique_ptr<design_patterns::ThreadPoolExecutor> tpe_;      
            Input last_input_;

            /**
             * Method to load the config file
             * @param file The file to be parsed
             */
            void loadConfig(const std::string& file);

            /**
             * Loop maintianing the connection to clients
             */
            void connectionHandler();

            void createOutputJson(const rpicomponents::mpu_angles& vals, const rpicomponents::GPSCoordinates& c, json& j);


            void processServerRead(std::string& buf, const std::string& delimiter);

    };
}

#endif