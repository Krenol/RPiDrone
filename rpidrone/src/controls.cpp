#include "controls.hpp"
#include <future>
#include <cmath>
#include <stdio.h>
#include <Eigen/Dense>
#include <unistd.h>
#include "easylogging++.h"
#include "globals.hpp"
#include "misc.hpp"

#define CONTROL_LOG(LEVEL) CLOG(LEVEL, "controls") //define controls log
#define PID_LOG(LEVEL) CLOG(LEVEL, "pid")          //define controls log
namespace drone
{
    Controls::Controls(const ControlsStruct &controls, const SensorsStruct &sensors) : controls_{controls}
    {
        CONTROL_LOG(INFO) << "Initializing controls...";
        initEscs(controls.escs);
        initControllers(controls.escs);
        throttle_ = controls_.escs.idle;
        sensors_ = std::make_unique<Sensors>(sensors);
        throttle_factor_ = (controls_.escs.max - controls_.escs.idle - controls_.escs.max_diff) / drone::THROTTLE_UPPER_BOUND;
        std::async(std::launch::async, [this]() { this->zeroAltitude(); });
        CONTROL_LOG(INFO) << "Initialized controls successfully";
        #if defined(PID_LOGS)
        PID_LOG(DEBUG) << "datetime;level;roll_s;roll_is;err_roll;pitch_s;pitch_is;err_pitch;yawn_s;yawn_is;err_yawn;lb;rb;lf;rf";
        #endif
    }

    

    void Controls::initControllers(const Escs &escs)
    {
        Eigen::MatrixXd p(3, 1), d(3, 1), i(3, 1);
        Eigen::VectorXd max(1), min(1), aw(1);
        p << escs.roll.k_p, escs.pitch.k_p, escs.yawn.k_p;
        d << escs.roll.k_d, escs.pitch.k_d, escs.yawn.k_d;
        i << escs.roll.k_i, escs.pitch.k_i, escs.yawn.k_i;
        aw << escs.roll.k_aw;
        min << -controls_.escs.max_diff;
        max << controls_.escs.max_diff;
        CONTROL_LOG(INFO) << "P: " << p;
        CONTROL_LOG(INFO) << "D: " << d;
        CONTROL_LOG(INFO) << "I: " << i;
        CONTROL_LOG(INFO) << "I_AW: " << aw;
        CONTROL_LOG(INFO) << "MIN: " << min;
        CONTROL_LOG(INFO) << "MAX: " << max;
        pid_lf_ = std::make_unique<controllers::PID_AW>(p, d, i, aw, min, max);
        pid_lb_ = std::make_unique<controllers::PID_AW>(p, d, i, aw, min, max);
        pid_rf_ = std::make_unique<controllers::PID_AW>(p, d, i, aw, min, max);
        pid_rb_ = std::make_unique<controllers::PID_AW>(p, d, i, aw, min, max);
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
        while(runs > 0) {
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
            f= std::async(std::launch::async, [this]() { this->startEsc(lf_); });
        }
        f.get();
    }

    void Controls::process_input(const Input &input)
    {
        const std::lock_guard<std::mutex> lock(mtx_);
        throttle_ = input.throttle * throttle_factor_ + controls_.escs.idle + controls_.escs.max_diff;
        roll_angle_s_ = input.joystick.offset * cos(input.joystick.degrees * M_PI / 180.0) * controls_.max_roll;
        pitch_angle_s_ = input.joystick.offset * sin(input.joystick.degrees * M_PI / 180.0) * controls_.max_pitch;
        yawn_vel_s_ = controls_.max_yawn * input.joystick.rotation / drone::ROTATION_UPPER_BOUND;
        client_pos_ = rpicomponents::GPSCoordinates(input.gps);
        CONTROL_LOG(DEBUG) << "set throttle to " << throttle_ << "; set roll angle to " << roll_angle_s_ << "; set pitch angle to " << pitch_angle_s_ << "; set yawn velocity to " << yawn_vel_s_;
    }

    int Controls::getThrottle()
    {
        return throttle_;
    }

    void Controls::getDroneCoordinates(rpicomponents::GPSCoordinates& c, int retires) 
    {
        sensors_->getDroneCoordinates(c, retires);
    }
    
    float Controls::getAltitude() 
    {
        return sensors_->getBarometricHeight() - zeroed_altitude_;
    }
    
    void Controls::control(control_values& vals)
    {
        const std::lock_guard<std::mutex> lock(mtx_);
        sensors_->getControlValues(vals);
        Eigen::VectorXd is(3), shld(3), lb, rb, lf, rf;
        // pos pitch_angle is when back is higher than front (drone flies forward)
        // pos roll_angle is when right side is higher than left side (drone flies left)
        // right side
        // right back
        is << -vals.pitch_angle, vals.roll_angle, -vals.z_vel;
        shld << -pitch_angle_s_, roll_angle_s_, -yawn_vel_s_;
        pid_rb_->calculate(is, shld, rb);
        // right front
        is << vals.pitch_angle, vals.roll_angle, vals.z_vel;
        shld << pitch_angle_s_, roll_angle_s_, yawn_vel_s_;
        pid_rf_->calculate(is, shld, rf);

        // left side
        // left back
        is << -vals.pitch_angle, -vals.roll_angle, vals.z_vel;
        shld << -pitch_angle_s_, -roll_angle_s_, yawn_vel_s_;
        pid_lb_->calculate(is, shld, lb);
        // left front
        is << vals.pitch_angle, -vals.roll_angle, -vals.z_vel;
        shld << pitch_angle_s_, -roll_angle_s_, -yawn_vel_s_;
        pid_lf_->calculate(is, shld, lf);
        #if defined(PID_LOGS)
        PID_LOG(INFO) << roll_angle_s_ << ";" << vals.roll_angle << ";" << roll_angle_s_ - vals.roll_angle << ";" << 
            pitch_angle_s_ << ";" << vals.pitch_angle << ";" << pitch_angle_s_ - vals.pitch_angle << ";" <<
            yawn_vel_s_ << ";" << vals.z_vel << ";" << yawn_vel_s_ - vals.z_vel << ";" <<
            (int)lb(0) << ";" << (int)rb(0) << ";" << (int)lf(0) << ";" << (int)rf(0);
        #endif
        lf_->SetOutputSpeed(throttle_ + lf(0));
        rb_->SetOutputSpeed(throttle_ + rb(0));
        rf_->SetOutputSpeed(throttle_ + rf(0));
        lb_->SetOutputSpeed(throttle_ + lb(0));
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