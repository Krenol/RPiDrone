#include "rpicomponents/rpicomponents.hpp"
#include <nlohmann/json.hpp>
#include <memory>
#include "pid.hpp"
#include <string>
#include "sensors.hpp"
#include "json_parser.hpp"
#include "input_parser.hpp"

#ifndef DRONE_CONTROLS_H
#define DRONE_CONTROLS_H

using json = nlohmann::json;

namespace drone {
    class Controls  {
        public:
            /**
             * Constructor
             * @param controls struct containing the controls array from the CONF_FILE
             * @param sensors struct containing the sensors part from the CONF_FILE
             */
            Controls(const ControlsStruct& controls, const SensorsStruct& sensors);

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
            void getDroneCoordinates(rpicomponents::GPSCoordinates& c, int retires = 100);

            /**
             * Method to get the barometric alitude based upon the start level
             * @returns the altitude in m
             */
            float getAltitude();
        private:
            const ControlsStruct controls_;
            std::unique_ptr<rpicomponents::Esc> lf_, rf_, lb_, rb_;
            std::mutex mtx_; 
            std::unique_ptr<PID<float>> pid_roll_rate_, pid_pitch_rate_, pid_yaw_rate_, pid_roll_output_, pid_pitch_output_, pid_yaw_output_;
            std::unique_ptr<Sensors> sensors_;
            rpicomponents::GPSCoordinates client_pos_;
            float zeroed_altitude_;

            void startEsc(const std::unique_ptr<rpicomponents::Esc>& esc);
            void calibrateEsc(const std::unique_ptr<rpicomponents::Esc>& esc);
            void initControllers(const Escs& escs);
            void initEscs(const Escs& escs);

            void zeroAltitude(int measurements = 100);
    };
}

#endif