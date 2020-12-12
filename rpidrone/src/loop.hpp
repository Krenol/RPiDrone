#include "rpisocket/rpisocket.hpp"
#include "subqueue.hpp"
#include "controls.hpp"
#include <memory>
#include <string>
#include <atomic>


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

            /**
             * Method to check whether socket is connected
             * @returns true if connected else false
             */
            bool hasConnection();

            /**
             * Method to calibrate all sensors
             */
            void calibrate();

            /**
             * Method to save calibartion to given path
             * @param path The path calibration file should be stored
             */
            void saveCalibration(const std::string& path);

            /**
             * Load calibration from file
             * @param path The path to the calibration file
             */
            void loadCalibration(const std::string& path);
            
        private:
            std::unique_ptr<rpicomponents::Led> on_led_, status_led_;
            std::unique_ptr<rpicomponents::Button> on_btn_;
            std::unique_ptr<rpisocket::WiFiServer> server_;
            std::unique_ptr<Controls> controls_;
            json config_;
            std::unique_ptr<drone::SubscriberQueue<std::string>> readq_, writeq_;
            std::thread conn_thread_;
            std::atomic_bool thread_on_;
            

            /**
             * Method to load the config file
             * @param file The file to be parsed
             */
            void loadConfig(const std::string& file);

            /**
             * Process newly read input
             * @param read The read string
             */
            void processInput(const std::string& read);

            /**
             * Loop maintianing the connection to clients
             */
            void connectionHandler();


            void processServerRead(std::string& buf, const std::string& delimiter);

    };
}

#endif