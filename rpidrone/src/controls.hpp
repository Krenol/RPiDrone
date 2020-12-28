#include "rpicomponents/rpicomponents.hpp"
#include <nlohmann/json.hpp>
#include <memory>
#include "controllers/controllers.hpp"
#include <string>
#include "controllers/controllers.hpp"
#include <atomic>
#include "sensors.hpp"

#ifndef DRONE_CONTROLS_H
#define DRONE_CONTROLS_H

using json = nlohmann::json;

namespace drone {
    class Controls  {
        public:
            /**
             * Constructor
             * @param controls JSON containing the controls array from the CONF_FILE
             * @param sensors JSON containing the sensors part from the CONF_FILE
             */
            Controls(const json& controls, const json& sensors);

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

            /**
             * Control method to control ESCs etc based on sensors and input
             * @param vals The control values struct
             */
            void control(control_values& vals);

            /**
             * Method to idle motors
             */
            void idle();

            /**
             * Method to turn motors off
             */
            void motorsOff();

            /**
             * Get the current location of the Drone via the GPS Sensor
             * @param c The struct to store the GPS coords
             * @param retries The amount of retries to try reading the GPS pos from serial
             */
            void getDroneCoordinates(rpicomponents::GPSCoordinates& c, int retires = 100);

            /**
             * Method to get the barometric alitude based upon the start level
             * @returns the altitude in m
             */
            float getAltitude();
        private:
            std::unique_ptr<rpicomponents::Esc> lf_, rf_, lb_, rb_;
            std::mutex mtx_; 
            std::unique_ptr<controllers::PID_AW> pid_lf_, pid_rf_, pid_rb_, pid_lb_;
            const int idle_, esc_max_, esc_min_, max_diff_;
            const float max_roll_angle_{20}, max_pitch_angle_{20}, max_yawn_vel_ {5};
            std::atomic_int throttle_{0};
            std::atomic<float> roll_angle_s_{0}, pitch_angle_s_{0}, yawn_vel_s_{0};
            std::unique_ptr<Sensors> sensors_;
            const bool calibrate_escs_;
            rpicomponents::GPSCoordinates client_pos_;
            float altitude_0_;

            void startEsc(const std::unique_ptr<rpicomponents::Esc>& esc);
            void calibrateEsc(const std::unique_ptr<rpicomponents::Esc>& esc);
            void initControllers(const json& controls);
            void initEscs(const json& controls);

            void zeroAltitude(int measurements = 100);

            void parseThrottle(const json& input);

            void parseJoystick(const json& input);

            void parseGPS(const json& input);
    };
}

#endif