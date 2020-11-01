#include "rpisocket/rpisocket.hpp"
#include "subqueue.hpp"
#include "sensorics.hpp"
#include "controls.hpp"
#include <memory>
#include <string>


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
             * Method that is processing the btn press
             */
            void awaitBtnPress();

            /**
             * Await button press and startup everything
             */
            void startupDrone();
            
        private:
            std::unique_ptr<rpicomponents::Led> on_led_, status_led_;
            std::unique_ptr<rpicomponents::Button> on_btn_;
            std::unique_ptr<rpisocket::Server> server_;
            std::unique_ptr<Controls> controls_;
            std::unique_ptr<Sensorics> sensorics_;
            json config_;
            std::shared_ptr<drone::SubscriberQueue<std::string>> subq_;

            /**
             * Method to load the config file
             * @param file The file to be parsed
             * @returns the parsed config file as JSON
             */
            json loadConfig(const std::string& file);

            /**
             * Process newly read input
             * @param read The read string
             * @param j The json to hold the parsed JSON
             */
            void processInput(const std::string& read, json& j);

    };
}

#endif