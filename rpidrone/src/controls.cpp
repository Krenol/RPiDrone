#include "controls.hpp"
#include <future>
#include <cmath>
#include <stdio.h>
#include <Eigen/Dense>
#include <unistd.h>
#include "easylogging++.h"
#include "globals.hpp"

#define CONTROL_LOG(LEVEL) CLOG(LEVEL, "controls") //define controls log
#if defined(PID_LOGS)
#define PID_LOG(LEVEL) CLOG(LEVEL, "pid")          //define pid log
#endif
namespace drone
{
    Controls::Controls(const ControlsStruct &controls, const SensorsStruct &sensors) : controls_{controls}
    {
        CONTROL_LOG(INFO) << "Initializing controls...";
        initEscs(controls.escs);
        initControllers(controls.escs);
        throttle_ = controls_.escs.idle;
        sensors_ = std::make_unique<Sensors>(sensors);
        throttle_factor_ = (controls_.escs.max - controls_.escs.idle) / drone::THROTTLE_UPPER_BOUND;
        std::async(std::launch::async, [this]() { this->zeroAltitude(); });
        CONTROL_LOG(INFO) << "Initialized controls successfully";
        #if defined(PID_LOGS)
        PID_LOG(DEBUG) << "datetime;level;roll_s;roll_is;err_roll;pitch_s;pitch_is;err_pitch;vel_x_is;vel_x_s;err_vel_x;vel_y_is;vel_y_s;err_vel_y;vel_z_is;vel_z_s;err_vel_z;lb;rb;lf;rf;throttle";
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

    void Controls::zeroAltitude(int measurements)
    {
        int runs = measurements;
        zeroed_altitude_ = 0;
        while (runs > 0)
        {
            zeroed_altitude_ += sensors_->getBarometricHeight();
            --runs;
            usleep(100);
        }
        zeroed_altitude_ /= measurements;
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
    }

    void Controls::process_input(const Input &input)
    {
        const std::lock_guard<std::mutex> lock(mtx_);
        throttle_ = input.throttle * throttle_factor_ + controls_.escs.idle;
        roll_angle_s_ = input.joystick.offset * cos(input.joystick.degrees * M_PI / 180.0) * controls_.max_roll;
        pitch_angle_s_ = input.joystick.offset * sin(input.joystick.degrees * M_PI / 180.0) * controls_.max_pitch;
        yaw_vel_s_ = controls_.max_yawn * input.joystick.rotation / drone::ROTATION_UPPER_BOUND;
        client_pos_ = rpicomponents::GPSCoordinates(input.gps);
        CONTROL_LOG(DEBUG) << "set throttle to " << throttle_ << "; set roll angle to " << roll_angle_s_ << "; set pitch angle to " << pitch_angle_s_ << "; set yawn velocity to " << yaw_vel_s_;
    }

    int Controls::getThrottle()
    {
        return throttle_;
    }

    void Controls::getDroneCoordinates(rpicomponents::GPSCoordinates &c, int retires)
    {
        sensors_->getDroneCoordinates(c, retires);
    }

    float Controls::getAltitude()
    {
        return sensors_->getBarometricHeight() - zeroed_altitude_;
    }

    void Controls::control(control_values &vals)
    {
        float roll_out, pitch_out, yaw_out, roll_rate, pitch_rate, yaw_rate;
        int lb, rb, lf, rf;

        const std::lock_guard<std::mutex> lock(mtx_);
        sensors_->getControlValues(vals);

        roll_rate = pid_roll_rate_->control(vals.roll_angle, roll_angle_s_);
        pitch_rate = pid_pitch_rate_->control(vals.pitch_angle, pitch_angle_s_);
        yaw_rate = yaw_vel_s_;

        roll_out = pid_roll_output_->control(vals.x_vel, roll_rate);
        pitch_out = pid_pitch_output_->control(vals.y_vel, pitch_rate);
        yaw_out = pid_yaw_output_->control(vals.z_vel, yaw_rate);

        rf = roll_out - pitch_out + yaw_out;
        lf = -roll_out - pitch_out - yaw_out;
        rb = roll_out + pitch_out - yaw_out;
        lb = -roll_out + pitch_out + yaw_out; 

        #if defined(PID_LOGS)
        PID_LOG(INFO) << roll_angle_s_ << ";" << vals.roll_angle << ";" << roll_angle_s_ - vals.roll_angle<< ";" << pitch_angle_s_ << ";" << vals.pitch_angle << ";" << pitch_angle_s_ - vals.pitch_angle << ";" << vals.x_vel << ";" << roll_rate << ";" << vals.x_vel - roll_rate << ";" << vals.y_vel << ";" << pitch_rate << ";" << vals.y_vel - pitch_rate << ";" << vals.z_vel  << ";" << yaw_vel_s_ << ";" << yaw_vel_s_ - vals.z_vel << ";" << lb << ";" << rb << ";" << lf << ";" << rf << ";" << throttle_;
        #endif
        lf_->SetOutputSpeed(throttle_ + lf);
        rb_->SetOutputSpeed(throttle_ + rb);
        rf_->SetOutputSpeed(throttle_ + rf);
        lb_->SetOutputSpeed(throttle_ + lb);
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

    void Controls::idle()
    {
        throttle_ = controls_.escs.idle;
    }

    void Controls::motorsOff()
    {
        throttle_ = 0;
    }
} // namespace drone