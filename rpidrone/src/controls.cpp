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
    Controls::Controls(const json &controls, const json &sensors) : idle_{controls.at("escs").at("idle")}, esc_max_{controls.at("escs").at("max")}, esc_min_{controls.at("escs").at("min")}, max_diff_{controls.at("escs").at("controllers").at("max_diff")}, max_roll_angle_{controls.at("max_roll_angle")}, max_pitch_angle_{controls.at("max_pitch_angle")}, max_yawn_vel_{controls.at("max_yawn_velocity")}, calibrate_escs_{controls.at("escs").at("calibrate")}
    {
        CONTROL_LOG(INFO) << "Initializing controls...";
        initEscs(controls);
        initControllers(controls);
        throttle_ = idle_;
        sensors_ = std::make_unique<Sensors>(sensors);
        
        std::async(std::launch::async, [this]() { this->zeroAltitude(); });
        PID_LOG(DEBUG) << "datetime;level;roll_s;roll_is;err_roll;pitch_s;pitch_is;err_pitch;yawn_s;yawn_is;err_yawn;lb;rb;lf;rf";
        CONTROL_LOG(INFO) << "Initialized controls successfully";
    }

    

    void Controls::initControllers(const json &controls)
    {
        auto vals = controls.at("escs").at("controllers");
        Eigen::MatrixXd p(3, 1), d(3, 1), i(3, 1);
        Eigen::VectorXd max(1), min(1), aw(1);
        p << vals.at("k_p").at("roll"), vals.at("k_p").at("pitch"), vals.at("k_p").at("yawn");
        d << vals.at("k_d").at("roll"), vals.at("k_d").at("pitch"), vals.at("k_d").at("yawn");
        i << vals.at("k_i").at("roll"), vals.at("k_i").at("pitch"), vals.at("k_i").at("yawn");
        aw << vals.at("k_aw");
        min << -max_diff_;
        max << max_diff_;
        pid_lf_ = std::make_unique<controllers::PID_AW>(p, d, i, aw, min, max);
        pid_lb_ = std::make_unique<controllers::PID_AW>(p, d, i, aw, min, max);
        pid_rf_ = std::make_unique<controllers::PID_AW>(p, d, i, aw, min, max);
        pid_rb_ = std::make_unique<controllers::PID_AW>(p, d, i, aw, min, max);
    }

    void Controls::initEscs(const json &controls)
    {
        auto esc_pins = controls.at("escs").at("pins");
        json val;
        std::string pos;
        for (auto esc : esc_pins.items())
        {
            val = esc.value();
            pos = val.at("pos");
            if (pos == "rb")
            {
                rb_ = std::make_unique<rpicomponents::Esc>(val.at("pin"), esc_min_, esc_max_);
            }
            else if (pos == "lb")
            {
                lb_ = std::make_unique<rpicomponents::Esc>(val.at("pin"), esc_min_, esc_max_);
            }
            else if (pos == "rf")
            {
                rf_ = std::make_unique<rpicomponents::Esc>(val.at("pin"), esc_min_, esc_max_);
            }
            else if (pos == "lf")
            {
                lf_ = std::make_unique<rpicomponents::Esc>(val.at("pin"), esc_min_, esc_max_);
            }
        }
    }
    
    void Controls::zeroAltitude(int measurements) 
    {
        int runs = measurements;
        altitude_0_ = 0;
        while(runs > 0) {
            altitude_0_ += sensors_->getBarometricHeight();
            --runs;
            usleep(100);
        }
        altitude_0_ /= measurements;
    }

    void Controls::startMotors()
    {
        
        if (calibrate_escs_)
        {
            std::async(std::launch::async, [this]() { this->calibrateEsc(rb_); });
            std::async(std::launch::async, [this]() { this->calibrateEsc(rf_); });
            std::async(std::launch::async, [this]() { this->calibrateEsc(lb_); });
            auto h = std::async(std::launch::async, [this]() { this->calibrateEsc(lf_); });
            h.get(); // we wait for the last motor to finish it's startup
        }
        else
        {

            std::async(std::launch::async, [this]() { this->startEsc(rb_); });
            std::async(std::launch::async, [this]() { this->startEsc(rf_); });
            std::async(std::launch::async, [this]() { this->startEsc(lb_); });
            auto h = std::async(std::launch::async, [this]() { this->startEsc(lf_); });
            h.get(); // we wait for the last motor to finish it's startup
        }
    }

    void Controls::process_input(const json &input)
    {
        const std::lock_guard<std::mutex> lock(mtx_);
        parseThrottle(input);
        parseJoystick(input);
        parseGPS(input);
    }

    void Controls::parseThrottle(const json &input)
    {
        if (JSON_EXISTS(input, "throttle"))
        {
            int t = BOUND<int>(input.at("throttle"), 0, 100);
            throttle_ = (t * (esc_max_ - idle_ - max_diff_) / 100) + idle_ + max_diff_;
            CONTROL_LOG(INFO) << "set throttle to " << throttle_;
        }
    }

    void Controls::parseJoystick(const json &input)
    {
        if (JSON_EXISTS(input, "joystick"))
        {
            auto j = input.at("joystick");
            if (JSON_EXISTS(j, "offset") && JSON_EXISTS(j, "degrees"))
            {
                float offset = BOUND<float>(j.at("offset"), 0, 1.0);
                float degrees = BOUND<float>(j.at("degrees"), -360.0, 360.0);
                roll_angle_s_ = offset * cos(degrees * M_PI / 180.0) * max_roll_angle_;
                pitch_angle_s_ = offset * sin(degrees * M_PI / 180.0) * max_pitch_angle_;
                CONTROL_LOG(INFO) << "set roll angle to " << roll_angle_s_ << " and pitch angle to " << pitch_angle_s_;
            }
            if (JSON_EXISTS(j, "rotation")) {
                float rot = BOUND<float>(j.at("rotation"), -100.0, 100.0);
                yawn_vel_s_ = max_yawn_vel_ * rot / 100.0;
                CONTROL_LOG(INFO) << "set yawn velocity to " << yawn_vel_s_;
            }
        }
    }

    void Controls::parseGPS(const json &input)
    {
        if (JSON_EXISTS(input, "gps"))
        {
            auto j = input.at("gps");
            if (JSON_EXISTS(j, "altitude"))
            {
                client_pos_.altitude = j.at("altitude");
            }
            if (JSON_EXISTS(j, "latitude"))
            {
                client_pos_.latitude = j.at("latitude");
            }
            if (JSON_EXISTS(j, "longitude"))
            {
                client_pos_.longitude = j.at("longitude");
            }
        }
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
        return sensors_->getBarometricHeight() - altitude_0_;
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

        PID_LOG(INFO) << roll_angle_s_ << ";" << vals.roll_angle << ";" << roll_angle_s_ - vals.roll_angle << ";" << 
            pitch_angle_s_ << ";" << vals.pitch_angle << ";" << pitch_angle_s_ - vals.pitch_angle << ";" <<
            yawn_vel_s_ << ";" << vals.z_vel << ";" << yawn_vel_s_ - vals.z_vel << ";" <<
            (int)lb(0) << ";" << (int)rb(0) << ";" << (int)lf(0) << ";" << (int)rf(0);
        lf_->SetOutputSpeed(throttle_ + lf(0));
        rf_->SetOutputSpeed(throttle_ + rf(0));
        lb_->SetOutputSpeed(throttle_ + lb(0));
        rb_->SetOutputSpeed(throttle_ + rb(0));
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
        lf_->SetOutputSpeed(idle_);
        rf_->SetOutputSpeed(idle_);
        lb_->SetOutputSpeed(idle_);
        rb_->SetOutputSpeed(idle_);
    }

    void Controls::motorsOff()
    {
        lf_->SetOutputSpeed(0);
        rf_->SetOutputSpeed(0);
        lb_->SetOutputSpeed(0);
        rb_->SetOutputSpeed(0);
    }
} // namespace drone