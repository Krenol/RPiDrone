#include "rpicomponents/rpicomponents.hpp"
#include <memory>
#include <nlohmann/json.hpp>
#include "json_parser.hpp"

#ifndef DRONE_SENSORICS_H
#define DRONE_SENSORICS_H

using json = nlohmann::json;

namespace drone {
    struct Distances {
        float dist_left {0}, dist_right {0}, dist_top {0}, dist_bottom {0}, dist_front {0}, dist_back {0};
    };

    struct control_values {
        float roll_angle, pitch_angle, yaw_angle, z_vel, x_vel, y_vel;
    };

    class Sensors {
        public:
            /**
             * Constructor
             * @param sensors struct holding the sensors values
             * @param unit Unit for everything handled inside the sensors
             */
            Sensors(const SensorsStruct &sensors, rpicomponents::DISTANCE_UNIT unit = rpicomponents::UNIT_MM);

            /**
             * Method to calibrate all sensors that can be calibrated
             * @param measurements Amount of measurements for calibration
            */
            void calibrate(int measurements);

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

            /**
             * Method to get all values needed for the controllers
             * @param vals: The struct to store the control values
             */
            void getControlValues(control_values& vals);

            /**
             * Get the current location of the Drone via the GPS Sensor
             * @param c The struct to store the GPS coords
             * @param retries The amount of retries to try reading the GPS pos from serial
             */
            void getDroneCoordinates(rpicomponents::GPSCoordinates& c, int retires = 10);

            /**
             * Method to get the barometric height above sea level from the BMP180
             * @returns the barometric height in m
             */
            float getBarometricHeight();
        
        private:
            const rpicomponents::DISTANCE_UNIT unit_;
            //std::unique_ptr<rpicomponents::UltrasonicSensor> uss_left_, uss_right_, uss_top_, uss_bottom_, uss_front_, uss_back_;
            std::unique_ptr<rpicomponents::GpsNeo6MV2> gps_;
            std::unique_ptr<rpicomponents::Ahrs> ahrs_;
            #if defined(US)
            std::unique_ptr<rpicomponents::Bmp180<std::chrono::microseconds>> bmp_;
            std::unique_ptr<rpicomponents::MPU6050<std::chrono::microseconds>> mpu_;
            #elif defined(NS)
            std::unique_ptr<rpicomponents::Bmp180<std::chrono::nanoseconds>> bmp_;
            std::unique_ptr<rpicomponents::MPU6050<std::chrono::nanoseconds>> mpu_;
            #else
            std::unique_ptr<rpicomponents::Bmp180<std::chrono::milliseconds>> bmp_;
            std::unique_ptr<rpicomponents::MPU6050<std::chrono::milliseconds>> mpu_;
            #endif
            const SensorsStruct data_;
            

    };
}

#endif