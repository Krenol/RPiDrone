#include "rpicomponents/rpicomponents.hpp"
#include <memory>

#ifndef DRONE_SENSORICS_H
#define DRONE_SENSORICS_H

namespace drone {
    struct Distances {
        float dist_left {0}, dist_right {0}, dist_top {0}, dist_bottom {0}, dist_front {0}, dist_back {0};
    };

    class Sensorics {
        public:
            Sensorics();
            Sensorics(rpicomponents::DISTANCE_UNIT unit);

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
        
        private:
            const rpicomponents::DISTANCE_UNIT unit_;
            //std::unique_ptr<rpicomponents::UltrasonicSensor> uss_left_, uss_right_, uss_top_, uss_bottom_, uss_front_, uss_back_;
            std::unique_ptr<rpicomponents::MPU6050> mpu_;
            

    };
}

#endif