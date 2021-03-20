#include "controls.hpp"
#include <future>
#include <cmath>
#include <stdio.h>
#include <Eigen/Dense>
#include <unistd.h>
#include "logs/easylogging++.h"
#include "globals.hpp"

#define CONTROL_LOG(LEVEL) CLOG(LEVEL, "controls") //define controls log
#if defined(PID_LOGS)
#define PID_LOG(LEVEL) CLOG(LEVEL, "pid")          //define pid log
#endif
namespace drone
{
    Controls::Controls(const ControlsStruct &controls) : controls_{controls}
    {
        CONTROL_LOG(INFO) << "Initializing controls...";
        initEscs(controls.escs);
        initControllers(controls.escs);
        
        CONTROL_LOG(INFO) << "Initialized controls successfully";
        #if defined(PID_LOGS)
        PID_LOG(DEBUG) << "datetime;level;roll_s;roll_is;err_roll;pitch_s;pitch_is;err_pitch;vel_x_is;vel_x_s;err_vel_x;vel_y_is;vel_y_s;err_vel_y;vel_z_is;vel_z_s;err_vel_z;lb;rb;lf;rf;throttle;roll_out;pitch_out;yaw_out";
        #endif
    }

    void Controls::initControllers(const Escs &escs)
    {
        pid_roll_rate_ = std::make_unique<PID<float>>(escs.roll_rate.k_p, escs.roll_rate.k_d, escs.roll_rate.k_i, escs.roll_rate.k_aw, escs.roll_rate.min, escs.roll_rate.max);
        pid_pitch_rate_ = std::make_unique<PID<float>>(escs.pitch_rate.k_p, escs.pitch_rate.k_d, escs.pitch_rate.k_i, escs.pitch_rate.k_aw, escs.pitch_rate.min, escs.pitch_rate.max);
        pid_yaw_rate_ = std::make_unique<PID<float>>(escs.yaw_rate.k_p, escs.yaw_rate.k_d, escs.yaw_rate.k_i, escs.yaw_rate.k_aw, escs.yaw_rate.min, escs.yaw_rate.max);
        pid_roll_output_ = std::make_unique<PID<float>>(escs.roll_output.k_p, escs.roll_output.k_d, escs.roll_output.k_i, escs.roll_output.k_aw, escs.roll_output.min, escs.roll_output.max);
        pid_pitch_output_ = std::make_unique<PID<float>>(escs.pitch_output.k_p, escs.pitch_output.k_d, escs.pitch_output.k_i, escs.pitch_output.k_aw, escs.pitch_output.min, escs.pitch_output.max);
        pid_yaw_output_ = std::make_unique<PID<float>>(escs.yaw_output.k_p, escs.yaw_output.k_d, escs.yaw_output.k_i, escs.yaw_output.k_aw, escs.yaw_output.min, escs.yaw_output.max);

    }

    void Controls::initEscs(const Escs &escs)
    {
        rb_ = std::make_unique<rpicomponents::Esc>(escs.pin_rb, controls_.escs.min, controls_.escs.max);
        lb_ = std::make_unique<rpicomponents::Esc>(escs.pin_lb, controls_.escs.min, controls_.escs.max);
        rf_ = std::make_unique<rpicomponents::Esc>(escs.pin_rf, controls_.escs.min, controls_.escs.max);
        lf_ = std::make_unique<rpicomponents::Esc>(escs.pin_lf, controls_.escs.min, controls_.escs.max);
    }

    void Controls::startMotors()
    {
        std::future<void> f;
        if (controls_.escs.calibrate)
        {
            std::async(std::launch::async, [this]() { this->calibrateEsc(rb_); });
            std::async(std::launch::async, [this]() { this->calibrateEsc(rf_); });
            std::async(std::launch::async, [this]() { this->calibrateEsc(lb_); });
            f = std::async(std::launch::async, [this]() { this->calibrateEsc(lf_); });
        }
        else
        {
            std::async(std::launch::async, [this]() { this->startEsc(rb_); });
            std::async(std::launch::async, [this]() { this->startEsc(rf_); });
            std::async(std::launch::async, [this]() { this->startEsc(lb_); });
            f = std::async(std::launch::async, [this]() { this->startEsc(lf_); });
        }
        f.get();
        sleep(5); //give motors 5s
    }

    // void Controls::getDroneCoordinates(rpicomponents::GPSCoordinates &c, int retires)
    // {
    //     sensors_->getDroneCoordinates(c, retires);
    // }

    void Controls::control(SensorData& sensorData, const UserInput &userInput)
    {
        float roll_out, pitch_out, yaw_out, roll_rate, pitch_rate, yaw_rate;
        int lb, rb, lf, rf;

        const std::lock_guard<std::mutex> lock(mtx_);

        roll_rate = pid_roll_rate_->control(sensorData.roll_angle, userInput.roll_angle);
        pitch_rate = pid_pitch_rate_->control(sensorData.pitch_angle, userInput.pitch_angle);
        yaw_rate = userInput.yaw_vel;

        roll_out = pid_roll_output_->control(sensorData.x_vel, roll_rate);
        pitch_out = pid_pitch_output_->control(sensorData.y_vel, pitch_rate);
        yaw_out = pid_yaw_output_->control(sensorData.z_vel, yaw_rate);

        rf = roll_out - pitch_out + yaw_out;
        lf = -roll_out - pitch_out - yaw_out;
        rb = roll_out + pitch_out - yaw_out;
        lb = -roll_out + pitch_out + yaw_out; 

        #if defined(PID_LOGS)
        PID_LOG(INFO) << userInput.roll_angle << ";" << sensorData.roll_angle << ";" << userInput.roll_angle - sensorData.roll_angle<< ";" << userInput.pitch_angle << ";" << sensorData.pitch_angle << ";" << userInput.pitch_angle - sensorData.pitch_angle << ";" << sensorData.x_vel << ";" << roll_rate << ";" << sensorData.x_vel - roll_rate << ";" << sensorData.y_vel << ";" << pitch_rate << ";" << sensorData.y_vel - pitch_rate << ";" << sensorData.z_vel  << ";" << userInput.yaw_vel << ";" << userInput.yaw_vel - sensorData.z_vel << ";" << lb << ";" << rb << ";" << lf << ";" << rf << ";" << userInput.throttle << ";" << roll_out << ";" << pitch_out << ";" << yaw_out;
        #endif
        lf_->SetOutputSpeed(userInput.throttle + lf);
        rb_->SetOutputSpeed(userInput.throttle + rb);
        rf_->SetOutputSpeed(userInput.throttle + rf);
        lb_->SetOutputSpeed(userInput.throttle + lb);
    }

    void Controls::startEsc(const std::unique_ptr<rpicomponents::Esc> &esc)
    {
        esc->Arm();
        esc->SetOutputSpeed(0);
        sleep(1);
    }

    void Controls::calibrateEsc(const std::unique_ptr<rpicomponents::Esc> &esc)
    {
        esc->Calibrate(false);
        esc->SetOutputSpeed(0);
        sleep(1);
    }
} // namespace drone