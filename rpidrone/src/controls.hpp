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
             * @param controls JSON containing the controls array from the CONF_FILE
             * @param sensorics JSON containing the sensorics part from the CONF_FILE
             */
            Controls(const json& controls, const json& sensorics);

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
             * Control method to control ESCs etc based on sensorics and input
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
             * Method to calibrate all sensors
             */
            void calibrateSensors();

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

            /**
             * Get the current location of the Drone via the GPS Sensor
             * @param c The struct to store the GPS coords
             */
            void getDroneCoordinates(rpicomponents::GPSCoordinates& c);

        private:
            std::unique_ptr<rpicomponents::Esc> lf_, rf_, lb_, rb_;
            std::mutex mtx_; 
            std::unique_ptr<controllers::PID_AW> pid_lf_, pid_rf_, pid_rb_, pid_lb_;
            const int idle_, esc_max_, esc_min_, max_diff_;
            const float max_roll_angle_{20}, max_pitch_angle_{20}, max_yawn_vel_ {5};
            std::atomic_int throttle_{0};
            std::atomic<float> roll_angle_s_{0}, pitch_angle_s_{0}, yawn_vel_s_{0};
            std::unique_ptr<Sensorics> sensorics_;
            const bool calibrate_escs_, calibrate_sensors_;
            rpicomponents::GPSCoordinates client_pos_;

            void startEsc(const std::unique_ptr<rpicomponents::Esc>& esc);
            void calibrateEsc(const std::unique_ptr<rpicomponents::Esc>& esc);
            void initControllers(const json& controls);
            void initEscs(const json& controls);

            void parseThrottle(const json& input);

            void parseJoystick(const json& input);

            void parseGPS(const json& input);
    };
}

#endif