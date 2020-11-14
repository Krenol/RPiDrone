#include "rpicomponents/rpicomponents.hpp"
#include <nlohmann/json.hpp>
#include <memory>
#include "controllers/controllers.hpp"
#include <string>
#include "controllers/controllers.hpp"
#include <atomic>
#include "sensorics.hpp"

#ifndef DRONE_CONTROLS_H
#define DRONE_CONTROLS_H

using json = nlohmann::json;

namespace drone {
    class Controls  {
        public:
            /**
             * Constructor
             * @param controls JSON containing the controls array from the config.json
             * @param sensorics JSON containing the sensorics part from the config.json
             */
            Controls(const json& controls, const json& sensorics);

            /**
             * Method to fire up all motors
             * @param calibrate Flag to turn on/off sensor calibration on motor start
             */
            void startMotors(bool calibrate = false);

            /**
             * Method that set motor speeds and angles based on control input;
             * data is received via the queue
             * @param input The transmitted input
             */
            void process_input(const json& input);

            /**
             * Method that returns the current trhottle value
             */
            int getThrottle();

            /**
             * Control method to control ESCs etc based on sensorics and input
             */
            void control();

            /**
             * Method to idle motors
             */
            void idle();

            /**
             * Method to turn motors off
             */
            void motorsOff();

            /**
             * Method to calibrate all sensors
             */
            void calibrateSensors();

        private:
            std::unique_ptr<rpicomponents::Esc> lf_, rf_, lb_, rb_;
            std::mutex mtx_; 
            std::unique_ptr<controllers::PD> pid_lf_, pid_rf_, pid_rb_, pid_lb_;
            const int idle_, esc_max_, esc_min_, max_diff_;
            const float beta_max_{20}, gamma_max_{20};
            std::atomic_int throttle_{0};
            std::atomic<float> beta_s_{0}, gamma_s_{0};
            std::unique_ptr<Sensorics> sensorics_;

            void startEsc(const std::unique_ptr<rpicomponents::Esc>& esc);
            void initControllers(const json& controls);
            void initEscs(const json& controls);
    };
}

#endif