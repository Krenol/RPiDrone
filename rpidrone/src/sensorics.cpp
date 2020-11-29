#include "sensorics.hpp"
#include "utils/utils.hpp"
#include <iostream>
#include <nlohmann/json.hpp>
#include "easylogging++.h"
#include <fstream>

#define SENSOR_LOG(LEVEL) CLOG(LEVEL, "sensorics")   //define sensor log

namespace drone
{
    Sensorics::Sensorics(const json& sensorics, rpicomponents::DISTANCE_UNIT unit)  : unit_{unit}
    {
        
        mpu_ = std::make_unique<rpicomponents::MPU6050>(sensorics.at("mpu").at("address"), rpicomponents::G_4, rpicomponents::DPS_500);
        auto data = sensorics.at("mpu").at("kalman");
        mpu_kalman_conf c(data.at("c1"), data.at("c2"), data.at("r"), data.at("q11"), data.at("q12"), data.at("q21"), data.at("q22"));
        mpu_->SetKalmanConfig(c);
    }


    Distances Sensorics::getDistances() const
    {
        Distances dist;
        // dist.dist_front = uss_front_->MeasureDistance(rpicomponents::UNIT_M);
        return dist;
    }
    
    void Sensorics::getKalmanAngles(rpicomponents::mpu_angles& angles) 
    {
        mpu_->GetKalmanAngles(angles);
    }

    bool Sensorics::calibrate() 
    {
        mpu_->CalibrateAcceleration();
        mpu_->CalibrateGyro();
    }
    
    bool Sensorics::loadCalibration(const std::string& path) 
    {
        std::ifstream ifs(path);
        auto j = json::parse(ifs);
        mpu_->CalibrateFromJson(j);
    }
    
    bool Sensorics::storeCalibration(const std::string& path) 
    {
        nlohmann::json j;
        mpu_->GetOffsets(j);
        std::ofstream file(path);
        file << j;
    }
}