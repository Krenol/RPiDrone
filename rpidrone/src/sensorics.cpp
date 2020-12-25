#include "sensorics.hpp"
#include "utils/utils.hpp"
#include <iostream>
#include <nlohmann/json.hpp>
#include "easylogging++.h"
#include <fstream>
#include <Eigen/Dense>
#include "misc.hpp"

#define SENSOR_LOG(LEVEL) CLOG(LEVEL, "sensorics")   //define sensor log

namespace drone
{
    Sensorics::Sensorics(const json& sensorics, rpicomponents::DISTANCE_UNIT unit)  : unit_{unit}
    {
        
        mpu_ = std::make_unique<rpicomponents::MPU6050>(sensorics.at("mpu").at("address"), rpicomponents::G_4, rpicomponents::DPS_500);
        auto data = sensorics.at("mpu").at("kalman");
        mpu_kalman_conf mpu_conf(data.at("c1"), data.at("c2"), data.at("r"), data.at("q11"), data.at("q12"), data.at("q21"), data.at("q22"));
        mpu_->SetKalmanConfig(mpu_conf);
        gps_ = std::make_unique<rpicomponents::GpsNeo6MV2>(sensorics.at("gps").at("port"), sensorics.at("gps").at("baudrate"));
        mpu_filter_ = std::make_unique<utils::ExponentialFilter>(sensorics.at("mpu").at("exp_filter"), Eigen::VectorXd::Zero(1));
        bpm_ = std::make_unique<rpicomponents::Bmp180>(sensorics.at("bpm").at("address"), sensorics.at("bpm").at("accuracy"));
        data = sensorics.at("bpm").at("kalman");
        bmp_kalman_conf bpm_conf(data.at("c1"), data.at("c2"), data.at("q11"), data.at("q12"), data.at("q21"), data.at("q22"), sensorics.at("bpm").at("accuracy"));
        bpm_->SetKalmanConfig(bpm_conf);
        decimal_places_ = sensorics.at("decimal_places");
    }


    Distances Sensorics::getDistances() const
    {
        Distances dist;
        // dist.dist_front = uss_front_->MeasureDistance(rpicomponents::UNIT_M);
        return dist;
    }
    
    void Sensorics::getControlValues(control_values& vals) 
    {
        rpicomponents::mpu_angles angles;
        mpu_->GetKalmanAngles(angles);

        rpicomponents::mpu_data vel;
        mpu_->GetAngularVelocity(vel);

		Eigen::VectorXd filter_vals(1);
        filter_vals << vel.z;
        mpu_filter_->predict(filter_vals);
        vals.z_vel = ROUND<float>(filter_vals[0], decimal_places_);
        vals.pitch_angle = ROUND<float>(angles.pitch_angle, decimal_places_);
        vals.roll_angle = ROUND<float>(angles.roll_angle, decimal_places_);
    }
    
    void Sensorics::getDroneCoordinates(rpicomponents::GPSCoordinates& c, int retires) 
    {
        gps_->getCoordinates(c, retires);
    }
    
    float Sensorics::getBarometricHeight() 
    {
        return ROUND<float>(bpm_->getAltitudeKalman(), decimal_places_);
    }

    bool Sensorics::calibrate() 
    {
        mpu_->CalibrateAcceleration();
        mpu_->CalibrateGyro();
        return true;
    }
    
    bool Sensorics::loadCalibration(const std::string& path) 
    {
        bool success = false;
        try {
            std::ifstream ifs(path);
            auto j = json::parse(ifs);
            mpu_->CalibrateFromJson(j);
            success = true;
        } catch(...) {
            //nothing
        }
        return success;
    }
    
    bool Sensorics::storeCalibration(const std::string& path) 
    {
        bool success = false;
        try {
            nlohmann::json j;
            mpu_->GetOffsets(j);
            std::ofstream file(path);
            file << j;
            success = true;
        } catch(...) {
            //nothing
        }
        return success;
        
    }
}