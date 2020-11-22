#include "controls.hpp"
#include <future>
#include <cmath> 
#include <stdio.h> 
#include <Eigen/Dense>
#include <unistd.h>
#include "easylogging++.h"

#define CONTROL_LOG(LEVEL) CLOG(LEVEL, "controls") //define controls log
#define PID_LOG(LEVEL) CLOG(LEVEL, "pid") //define controls log
namespace drone
{
    Controls::Controls(const json& controls, const json& sensorics) : idle_ {controls.at("escs").at("idle")}, esc_max_ {controls.at("escs").at("max")}, esc_min_ {controls.at("escs").at("min")}, max_diff_{controls.at("escs").at("controllers").at("max_diff")}, beta_max_{controls.at("beta_max")}, gamma_max_{controls.at("gamma_max")}
    {
        initEscs(controls);
        initControllers(controls);
        throttle_ = idle_;
        sensorics_ = std::make_unique<Sensorics>(sensorics);
        PID_LOG(DEBUG) << "datetime;level;beta_s;beta_is;err_b;gamma_s;gamma_is;err_g;lb;rb;lf;rf";
    }

    void Controls::initControllers(const json& controls){
        auto vals = controls.at("escs").at("controllers");
        Eigen::MatrixXd p(2, 1), d(2,1), i(2,1);
        Eigen::VectorXd max(1), min(1), aw(1);
        p << vals.at("k_p"), vals.at("k_p");
        d << vals.at("k_d"), vals.at("k_d");
        i << vals.at("k_i"), vals.at("k_i");
        aw << vals.at("k_aw");
        min << -max_diff_;
        max << max_diff_;
        pid_lf_ = std::make_unique<controllers::PID_AW>(p, d, i, aw, min, max);
        pid_lb_ = std::make_unique<controllers::PID_AW>(p, d, i, aw, min, max);
        pid_rf_ = std::make_unique<controllers::PID_AW>(p, d, i, aw, min, max);
        pid_rb_ = std::make_unique<controllers::PID_AW>(p, d, i, aw, min, max);
    }

    void Controls::initEscs(const json& controls) 
    {
        auto esc_pins = controls.at("escs").at("pins");
        json val;
        std::string pos;
        for(auto esc : esc_pins.items()){
            val = esc.value();
            pos = val.at("pos");
            if(pos == "rb"){
                rb_ = std::make_unique<rpicomponents::Esc>(val.at("pin"), esc_min_, esc_max_);
            } else if(pos == "lb"){
                lb_ = std::make_unique<rpicomponents::Esc>(val.at("pin"), esc_min_, esc_max_);
            } else if(pos == "rf") {
                rf_ = std::make_unique<rpicomponents::Esc>(val.at("pin"), esc_min_, esc_max_);
            } else if (pos == "lf") {
                lf_ = std::make_unique<rpicomponents::Esc>(val.at("pin"), esc_min_, esc_max_);
            }
        }
    }
    
    void Controls::startMotors(bool calibrate) 
    {
        if(calibrate){
            calibrateSensors();
        }
        std::async(std::launch::async, [this](){this->startEsc(rb_);});
        std::async(std::launch::async, [this](){this->startEsc(rf_);});
        std::async(std::launch::async, [this](){this->startEsc(lb_);});
        auto h = std::async(std::launch::async, [this](){this->startEsc(lf_);});
        h.get(); // we wait for the last motor to finish it's startup
    }
    
    void Controls::process_input(const json& input) 
    {
        const std::lock_guard<std::mutex> lock(mtx_);
        throttle_ = input.at("throttle");
        throttle_ =  (throttle_ * (esc_max_ - idle_ - max_diff_) / 100) + idle_ + max_diff_;
        float offset = input.at("joystick").at("offset");
        float degrees = input.at("joystick").at("degrees");
        beta_s_ = offset * cos(degrees * M_PI / 180.0) * beta_max_;
        gamma_s_ = offset * sin(degrees * M_PI / 180.0) * gamma_max_;
    }
    
    int Controls::getThrottle() 
    {
        return throttle_;
    }
    
    void Controls::control() 
    {
        const std::lock_guard<std::mutex> lock(mtx_);
        rpicomponents::mpu_angles angles;
        sensorics_->getKalmanAngles(angles);
        Eigen::VectorXd is(2), shld(2), lb, rb, lf, rf;
        is << angles.gamma, -angles.beta;
        shld << gamma_s_, -beta_s_;
        pid_lb_->calculate(is, shld, lb);
        is << -angles.gamma, -angles.beta;
        shld << -gamma_s_, -beta_s_;
        pid_rb_->calculate(is, shld, rb);
        is << angles.gamma, angles.beta;
        shld << gamma_s_, beta_s_;
        pid_rf_->calculate(is, shld, rf);
        is << -angles.gamma, angles.beta;
        shld << -gamma_s_, beta_s_;
        pid_lf_->calculate(is, shld, lf);

        PID_LOG(INFO) << beta_s_ << ";" << angles.beta << ";" << beta_s_ - angles.beta << ";" << gamma_s_ << ";" << angles.gamma << ";" << gamma_s_ - angles.gamma << ";" << lb << ";" << rb << ";" << lf << ";" << rf;
        lf_->SetOutputSpeed(throttle_ + lf(0));
        rf_->SetOutputSpeed(throttle_ + rf(0));
        lb_->SetOutputSpeed(throttle_ + lb(0));
        rb_->SetOutputSpeed(throttle_ + rb(0));
    }
    
    void Controls::startEsc(const std::unique_ptr<rpicomponents::Esc>& esc) 
    {
        esc->Arm();
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
    
    void Controls::calibrateSensors() 
    {
        sensorics_->calibrate();
    }
    
    
    void Controls::saveCalibration(const std::string& path) 
    {
        sensorics_->storeCalibration(path);
    }
    
    void Controls::loadCalibration(const std::string& path) 
    {
        sensorics_->loadCalibration(path);
    }
}