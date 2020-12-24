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
        mpu_kalman_conf c(data.at("c1"), data.at("c2"), data.at("r"), data.at("q11"), data.at("q12"), data.at("q21"), data.at("q22"));
        kalman_roll_angle_ = std::make_unique<MPU6050_Kalman>(c);
		kalman_pitch_angle_ = std::make_unique<MPU6050_Kalman>(c);
        gps_ = std::make_unique<rpicomponents::GpsNeo6MV2>(sensorics.at("gps").at("port"), sensorics.at("gps").at("baudrate"));
        mpu_filter_ = std::make_unique<utils::ExponentialFilter>(sensorics.at("mpu").at("exp_filter"), Eigen::VectorXd::Zero(5));
        bpm_ = std::make_unique<rpicomponents::Bmp180>(sensorics.at("bpm").at("address"), sensorics.at("bpm").at("accuracy"));
        Eigen::VectorXd h(1);
        h << bpm_->getAltitude();
        bpm_filter_ = std::make_unique<utils::ExponentialFilter>(sensorics.at("bpm").at("exp_filter"), h);
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
        mpu_->GetAccelerationAngles(angles);
		rpicomponents::mpu_data vel;
		mpu_->GetAngularVelocity(vel);
		Eigen::VectorXd u(1), z(1), filter_vals(5);
        filter_vals << vel.x, vel.y, vel.z, angles.roll_angle, angles.pitch_angle;
        mpu_filter_->predict(filter_vals);
		z << filter_vals[3];
		u << filter_vals[0];
		angles.roll_angle = kalman_roll_angle_->predict(z, u)[0];
		z << filter_vals[4];
		u << filter_vals[1];
		angles.pitch_angle = kalman_pitch_angle_->predict(z, u)[0];
        
        vals.z_vel = ROUND<float>(filter_vals[2], decimal_places_);
        vals.pitch_angle = ROUND<float>(angles.pitch_angle, decimal_places_);
        vals.roll_angle = ROUND<float>(angles.roll_angle, decimal_places_);
    }
    
    void Sensorics::getDroneCoordinates(rpicomponents::GPSCoordinates& c, int retires) 
    {
        gps_->getCoordinates(c, retires);
    }
    
    float Sensorics::getBarometricHeight() 
    {
        Eigen::VectorXd h(1);
        h << bpm_->getAltitude();
        bpm_filter_->predict(h);
        return ROUND<float>(h[0], decimal_places_);
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