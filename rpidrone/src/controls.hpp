#include "rpicomponents/rpicomponents.hpp"
#include <nlohmann/json.hpp>
#include <memory>
#include "controllers/controllers.hpp"
#include "design_patterns/design_patterns.hpp"
#include <string>

#ifndef DRONE_CONTROLS_H
#define DRONE_CONTROLS_H

using json = nlohmann::json;

namespace drone {
    class Controls : public 
    design_patterns::SubscriberQueue<std::string> {
        public:
            /**
             * Constructor
             * @param escs JSON containing the esc array from the config.json
             */
            Controls(const json& escs);

            /**
             * Method to fire up all motors
             */
            void startMotors();

            /**
             * Method that set motor speeds and angles based on control input;
             * data is received via the queue
             * @param input The transmitted input
             */
            void process_next(const std::string& input);

            /**
             * Method that returns the current trhottle value
             */
            int getThrottle();

        private:
            std::unique_ptr<rpicomponents::Esc> lf_, rf_, lb_, rb_;
            std::mutex mtx_; 
            const int idle_;
            int throttle_;

            void startEsc(const std::unique_ptr<rpicomponents::Esc>& esc);
    };
}

#endif