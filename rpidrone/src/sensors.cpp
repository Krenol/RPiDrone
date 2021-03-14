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
    Sensors::Sensors(const SensorsStruct &sensors, rpicomponents::DISTANCE_UNIT unit)  : data_{sensors}, unit_{unit}
    {
        SENSOR_LOG(INFO) << "Initializing sensors...";
        rpicomponents::bmp180_pressure_resolution res = (rpicomponents::bmp180_pressure_resolution)sensors.bmp.accuracy;
        #if defined(US)
        mpu_ = std::make_unique<rpicomponents::MPU6050<std::chrono::microseconds>>(sensors.mpu.address, rpicomponents::G_4, rpicomponents::DPS_500);
        bmp_ = std::make_unique<rpicomponents::Bmp180<std::chrono::microseconds>>(sensors.bmp.address, res);
        #elif defined(NS)
        mpu_ = std::make_unique<rpicomponents::MPU6050<std::chrono::nanoseconds>>(sensors.mpu.address, rpicomponents::G_4, rpicomponents::DPS_500);
        bmp_ = std::make_unique<rpicomponents::Bmp180<std::chrono::nanoseconds>>(sensors.bmp.address, res);
        #else
        mpu_ = std::make_unique<rpicomponents::MPU6050<std::chrono::milliseconds>>(sensors.mpu.address, rpicomponents::G_4, rpicomponents::DPS_500);
        bmp_ = std::make_unique<rpicomponents::Bmp180<std::chrono::milliseconds>>(sensors.bmp.address, res);
        #endif
        gps_ = std::make_unique<rpicomponents::GpsNeo6MV2>(sensors.gps.port, sensors.gps.baudrate);
        ahrs_ = std::make_unique<rpicomponents::Ahrs>(sensors.mpu.ahrs.beta);
        if (sensors.sensor_calibration.calibrate)
        {
            int runs = sensors.sensor_calibration.measurements;
            calibrate(runs);
            storeCalibration(CONF_DIR + "/" + MPU_CONF);
        }
        else
        {
            loadCalibration(CONF_DIR + "/" + MPU_CONF);
        }
        rpicomponents::mpu_kalman_angles_conf a_conf(sensors.mpu.kalman_angles.c1, sensors.mpu.kalman_angles.c2, sensors.mpu.kalman_angles.r, sensors.mpu.kalman_angles.q11, sensors.mpu.kalman_angles.q12, sensors.mpu.kalman_angles.q21, sensors.mpu.kalman_angles.q22);
        mpu_->SetKalmanConfig(a_conf);
        rpicomponents::mpu_kalman_vel_conf v_conf(sensors.mpu.kalman_velocity.r, sensors.mpu.kalman_velocity.q11, sensors.mpu.kalman_velocity.q22, sensors.mpu.kalman_velocity.q33, sensors.mpu.kalman_velocity.q44, sensors.mpu.kalman_velocity.q55, sensors.mpu.kalman_velocity.q66);
        mpu_->SetKalmanConfig(v_conf);
        rpicomponents::bmp_kalman_conf bmp_conf(sensors.bmp.kalman.c1, sensors.bmp.kalman.c2, sensors.bmp.kalman.q11, sensors.bmp.kalman.q12, sensors.bmp.kalman.q21, sensors.bmp.kalman.q22, sensors.bmp.accuracy);
        bmp_->SetKalmanConfig(bmp_conf);
        SENSOR_LOG(INFO) << "Initialized sensors successfully";
    }


    Distances Sensors::getDistances() const
    {
        Distances dist;
        // dist.dist_front = uss_front_->MeasureDistance(rpicomponents::UNIT_M);
        return dist;
    }


    
    void Sensors::getControlValues(SensorData& vals) 
    {
        rpicomponents::mpu_data vel;
        rpicomponents::mpu_angles angles;
        
        vals.x_vel = 0;
        vals.y_vel = 0;
        vals.z_vel = 0;
        vals.pitch_angle = 0;
        vals.roll_angle = 0;
        vals.yaw_angle = 0;
        for(auto i = 0; i < data_.control_measurements; i++) {
            mpu_->GetKalmanVelocity(vel);
            mpu_->GetKalmanAngles(angles);
            vals.x_vel += vel.x;
            vals.y_vel += vel.y;
            vals.z_vel += vel.z;
            vals.pitch_angle += angles.pitch_angle;
            vals.roll_angle += angles.roll_angle;
        }
        
        vals.x_vel = ROUND<float>(vals.x_vel / data_.control_measurements, data_.decimals);
        vals.y_vel = ROUND<float>(vals.y_vel / data_.control_measurements, data_.decimals);
        vals.z_vel = ROUND<float>(vals.z_vel / data_.control_measurements, data_.decimals);
        vals.pitch_angle = ROUND<float>(vals.pitch_angle / data_.control_measurements, data_.decimals);
        vals.roll_angle = ROUND<float>(vals.roll_angle / data_.control_measurements, data_.decimals);
    }
    
    void Sensors::getDroneCoordinates(rpicomponents::GPSCoordinates& c, int retires) 
    {
        gps_->getCoordinates(c, retires);
    }
    
    float Sensors::getBarometricHeight() 
    {
        return ROUND<float>(bmp_->getAltitudeKalman(), data_.decimals);
    }
    
    void Sensors::ahrs(SensorData& vals) 
    {
        rpicomponents::mpu_data vel, acc, acc1;
        rpicomponents::EulerAngles angles;
        
        vals.x_vel = 0;
        vals.y_vel = 0;
        vals.z_vel = 0;
        vals.pitch_angle = 0;
        vals.roll_angle = 0;
        vals.yaw_angle = 0;
        acc.x = 0;
        acc.y = 0;
        acc.z = 0;
        for(auto i = 0; i < data_.control_measurements; i++) {
            mpu_->GetAngularVelocity(vel);
            mpu_->GetAcceleration(acc1);
            vals.x_vel += vel.x;
            vals.y_vel += vel.y;
            vals.z_vel += vel.z;
            acc.x += acc1.x;
            acc.y += acc1.y;
            acc.z += acc1.z;
        }
        
        vals.x_vel = ROUND<float>(vals.x_vel / data_.control_measurements, data_.decimals);
        vals.y_vel = ROUND<float>(vals.y_vel / data_.control_measurements, data_.decimals);
        vals.z_vel = ROUND<float>(vals.z_vel / data_.control_measurements, data_.decimals);
        acc.x = ROUND<float>(acc.x / data_.control_measurements, data_.decimals);
        acc.y = ROUND<float>(acc.y / data_.control_measurements, data_.decimals);
        acc.z = ROUND<float>(acc.z / data_.control_measurements, data_.decimals);
        
        ahrs_->update(vals.x_vel, vals.y_vel, vals.z_vel, acc.x, acc.y, acc.z);
        ahrs_->getEulerAngles(angles);
        
        vals.pitch_angle = ROUND<float>(angles.pitch , data_.decimals);
        vals.roll_angle = ROUND<float>(angles.roll, data_.decimals);
        vals.yaw_angle = ROUND<float>(angles.yaw, data_.decimals);
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