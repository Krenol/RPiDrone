#include "sensorics.hpp"
#include "utils/utils.hpp"
#include <iostream>
#include <nlohmann/json.hpp>
#include "easylogging++.h"

#define SENSOR_LOG(LEVEL) CLOG(LEVEL, "sensorics")   //define sensor log

namespace drone
{
    Sensorics::Sensorics(const json& sensorics, rpicomponents::DISTANCE_UNIT unit)  : unit_{unit}
    {
        mpu_ = std::make_unique<rpicomponents::MPU6050>(sensorics.at("mpu"), rpicomponents::G_4, rpicomponents::DPS_500);
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
        
    }
    
    bool Sensorics::storeCalibration(const std::string& path) 
    {
        nlohmann::json j;
        mpu_->GetOffsets(j);
        std::cout << j.dump();
    }
}