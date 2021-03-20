#include "rpicomponents/rpicomponents.hpp"
#include <nlohmann/json.hpp>
#include <memory>
#include "pid.hpp"
#include <string>
#include "parsers/json_config_parser.hpp"
#include "user_input.hpp"

#ifndef DRONE_CONTROLS_H
#define DRONE_CONTROLS_H

using json = nlohmann::json;

namespace drone {
    struct SensorData {
        float roll_angle = 0, pitch_angle = 0, yaw_angle = 0, z_vel = 0, x_vel = 0, y_vel = 0;

        SensorData(float roll_angle = 0, float pitch_angle = 0, float yaw_angle = 0, float x_vel = 0, float y_vel = 0, float z_vel = 0) {
            this->roll_angle = roll_angle;
            this->pitch_angle = pitch_angle;
            this->yaw_angle = yaw_angle;
            this->z_vel = z_vel;
            this->x_vel = x_vel;
            this->y_vel = y_vel;
        }

        SensorData(const SensorData& sd) {
            this->roll_angle = sd.roll_angle;
            this->pitch_angle = sd.pitch_angle;
            this->yaw_angle = sd.yaw_angle;
            this->z_vel = sd.z_vel;
            this->x_vel = sd.x_vel;
            this->y_vel = sd.y_vel;
        }
    };

    class Controls  {
        public:
            /**
             * Constructor
             * @param controls struct containing the controls array from the CONF_FILE
             * @param sensors struct containing the sensors part from the CONF_FILE
             */
            Controls(const ControlsStruct& controls);

            /**
             * Method to fire up all motors
             */
            void startMotors();

            /**
             * Control method to control ESCs etc based on sensors and input
             * @param sensorData The SensorData struct containing sensor measurements
             * @param userInput The desired values needed for the controller
             */
            void control(SensorData& sensorData, const UserInput &userInput);

            /**
             * Get the current location of the Drone via the GPS Sensor
             * @param c The struct to store the GPS coords
             * @param retries The amount of retries to try reading the GPS pos from serial
             */
            // void getDroneCoordinates(rpicomponents::GPSCoordinates& c, int retires = 100);

        private:
            const ControlsStruct controls_;
            std::unique_ptr<rpicomponents::Esc> lf_, rf_, lb_, rb_;
            std::mutex mtx_; 
            std::unique_ptr<PID<float>> pid_roll_rate_, pid_pitch_rate_, pid_yaw_rate_, pid_roll_output_, pid_pitch_output_, pid_yaw_output_;
            

            void startEsc(const std::unique_ptr<rpicomponents::Esc>& esc);
            void calibrateEsc(const std::unique_ptr<rpicomponents::Esc>& esc);
            void initControllers(const Escs& escs);
            void initEscs(const Escs& escs);
    };
}

#endif