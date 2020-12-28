#include "sensors.hpp"
#include "utils/utils.hpp"
#include <iostream>
#include <nlohmann/json.hpp>
#include "easylogging++.h"
#include <fstream>
#include <Eigen/Dense>
#include "globals.hpp"
#include "misc.hpp"

#define SENSOR_LOG(LEVEL) CLOG(LEVEL, "sensors")   //define sensor log

namespace drone
{
    Sensors::Sensors(const json& sensors, rpicomponents::DISTANCE_UNIT unit)  : unit_{unit}
    {
        SENSOR_LOG(INFO) << "Initializing sensors...";
        json kals, data;
        mpu_ = std::make_unique<rpicomponents::MPU6050>(sensors.at("mpu").at("address"), rpicomponents::G_4, rpicomponents::DPS_500);
        bmp_ = std::make_unique<rpicomponents::Bmp180>(sensors.at("bpm").at("address"), sensors.at("bpm").at("accuracy"));
        gps_ = std::make_unique<rpicomponents::GpsNeo6MV2>(sensors.at("gps").at("port"), sensors.at("gps").at("baudrate"));
        data = sensors.at("calibration");
        bool calibrate_sensors = data.at("calibrate");
        if (calibrate_sensors)
        {
            int runs = data.at("measurements");
            calibrate(runs);
            storeCalibration(CONF_DIR + "/" + MPU_CONF);
        }
        else
        {
            loadCalibration(CONF_DIR + "/" + MPU_CONF);
        }
        kals = sensors.at("mpu").at("kalman");
        data = kals.at("angles");
        rpicomponents::mpu_kalman_angles_conf a_conf(data.at("c1"), data.at("c2"), data.at("r"), data.at("q11"), data.at("q12"), data.at("q21"), data.at("q22"));
        mpu_->SetKalmanConfig(a_conf);
        data = kals.at("velcoity");
        rpicomponents::mpu_kalman_vel_conf v_conf(data.at("r"), data.at("q11"), data.at("q22"), data.at("q33"), data.at("q44"), data.at("q55"), data.at("q66"));
        mpu_->SetKalmanConfig(v_conf);
        data = sensors.at("bpm").at("kalman");
        rpicomponents::bmp_kalman_conf bmp_conf(data.at("c1"), data.at("c2"), data.at("q11"), data.at("q12"), data.at("q21"), data.at("q22"), sensors.at("bpm").at("accuracy"));
        bmp_->SetKalmanConfig(bmp_conf);
        decimal_places_ = sensors.at("decimal_places");
        SENSOR_LOG(INFO) << "Initialized sensors successfully";
    }


    Distances Sensors::getDistances() const
    {
        Distances dist;
        // dist.dist_front = uss_front_->MeasureDistance(rpicomponents::UNIT_M);
        return dist;
    }
    
    void Sensors::getControlValues(control_values& vals) 
    {
        rpicomponents::mpu_angles angles;
        mpu_->GetKalmanAngles(angles);

        rpicomponents::mpu_data vel;
        mpu_->GetKalmanVelocity(vel);
        vals.z_vel = ROUND<float>(vel.z, decimal_places_);
        vals.pitch_angle = ROUND<float>(angles.pitch_angle, decimal_places_);
        vals.roll_angle = ROUND<float>(angles.roll_angle, decimal_places_);
        
    }
    
    void Sensors::getDroneCoordinates(rpicomponents::GPSCoordinates& c, int retires) 
    {
        gps_->getCoordinates(c, retires);
    }
    
    float Sensors::getBarometricHeight() 
    {
        return ROUND<float>(bmp_->getAltitudeKalman(), decimal_places_);
    }

    void Sensors::calibrate(int measurements) 
    {
        mpu_->CalibrateAcceleration(measurements);
        mpu_->CalibrateGyro(measurements);
        SENSOR_LOG(INFO) << "calibrated MPU successfully";
    }
    
    bool Sensors::loadCalibration(const std::string& path) 
    {
        bool success = false;
        try {
            std::ifstream ifs(path);
            auto j = json::parse(ifs);
            mpu_->CalibrateFromJson(j);
            success = true;
            SENSOR_LOG(INFO) << "loaded MPU calibration successfully";
        } catch(...) {
            //nothing
        }
        return success;
    }
    
    bool Sensors::storeCalibration(const std::string& path) 
    {
        bool success = false;
        try {
            nlohmann::json j;
            mpu_->GetOffsets(j);
            std::ofstream file(path);
            file << j;
            success = true;
            SENSOR_LOG(INFO) << "stored MPU calibration successfully";
        } catch(...) {
            //nothing
        }
        return success;
        
    }
}