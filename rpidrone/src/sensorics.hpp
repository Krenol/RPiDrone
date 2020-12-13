#include "rpicomponents/rpicomponents.hpp"
#include <memory>
#include <nlohmann/json.hpp>

#ifndef DRONE_SENSORICS_H
#define DRONE_SENSORICS_H

using json = nlohmann::json;

namespace drone {
    struct Distances {
        float dist_left {0}, dist_right {0}, dist_top {0}, dist_bottom {0}, dist_front {0}, dist_back {0};
    };

    struct control_values {
        float roll_angle, pitch_angle, z_vel;
    };

    class Sensorics {
        public:
            /**
             * Constructor
             * @param sensorics JSON holding the sensorics values
             * @param unit Unit for everything handled inside the sensorics
             */
            Sensorics(const json& sensors, rpicomponents::DISTANCE_UNIT unit = rpicomponents::UNIT_MM);

            /*
            Method to calibrate all sensors that can be calibrated
            @returns true if calibration was successful; else false
            */
            bool calibrate();

            /* 
            Method to laod a calibration file to restore previous calibration
            @param path: The path to the file
            @returns true if loading of calibration was successful, esle false
            */
            bool loadCalibration(const std::string& path);

            /* 
            Method to store calibration file to given path
            @param path: The path where file ist o be stored
            @returns true if storing was successful, else false
            */
            bool storeCalibration(const std::string& path);

            /*
            Method to get all distances measured by the USS
            @returns struct containing measured istances
            */
            Distances getDistances() const;         

            void getControlValues(control_values& vals);
        
        private:
            const rpicomponents::DISTANCE_UNIT unit_;
            //std::unique_ptr<rpicomponents::UltrasonicSensor> uss_left_, uss_right_, uss_top_, uss_bottom_, uss_front_, uss_back_;
            std::unique_ptr<rpicomponents::MPU6050> mpu_;
            std::unique_ptr<MPU6050_Kalman> kalman_roll_angle_, kalman_pitch_angle_;
            std::unique_ptr<utils::ExponentialFilter> mpu_filter_;
            

    };
}

#endif