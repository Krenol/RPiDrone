#include "rpicomponents/rpicomponents.hpp"
#include <nlohmann/json.hpp>
#include <memory>
#include "controllers/controllers.hpp"
#include <string>

#ifndef DRONE_CONTROLS_H
#define DRONE_CONTROLS_H

using json = nlohmann::json;

namespace drone {
    class Controls  {
        public:
            /**
             * Constructor
             * @param controls JSON containing the controls array from the config.json
             */
            Controls(const json& controls);

            /**
             * Method to fire up all motors
             */
            void startMotors();

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

            void control(const rpicomponents::mpu_angles& angles);

            /**
             * Method to idle motors
             */
            void idle();

            /**
             * Method to turn motors off
             */
            void motorsOff();

        private:
            std::unique_ptr<rpicomponents::Esc> lf_, rf_, lb_, rb_;
            std::mutex mtx_; 
            const int idle_, esc_max_, esc_min_;
            const double beta_max_{20}, gamma_max_{20};
            int throttle_;
            double beta_, gamma_;

            void startEsc(const std::unique_ptr<rpicomponents::Esc>& esc);
    };
}

#endif