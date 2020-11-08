#include "controls.hpp"
#include <future>
#include <cmath> 
#include <stdio.h> 

namespace drone
{
    Controls::Controls(const json& controls, const json& sensorics) : idle_ {controls.at("escs").at("idle")}, esc_max_ {controls.at("escs").at("max")}, esc_min_ {controls.at("escs").at("min")}, max_diff_{controls.at("escs").at("controllers").at("max_diff")}, beta_max_{controls.at("beta_max")}, gamma_max_{controls.at("gamma_max")}
    {
        initEscs(controls);
        initControllers(controls);
        throttle_ = idle_;
        sensorics_ = std::make_unique<Sensorics>(sensorics);
    }

    void Controls::initControllers(const json& controls){
        auto vals = controls.at("escs").at("controllers");
        pid_lb_ = std::make_unique<controllers::PD>(vals.at("k_p"), vals.at("k_d"), -max_diff_, max_diff_);
        pid_rb_ = std::make_unique<controllers::PD>(vals.at("k_p"), vals.at("k_d"), -max_diff_, max_diff_);
        pid_lf_ = std::make_unique<controllers::PD>(vals.at("k_p"), vals.at("k_d"), -max_diff_, max_diff_);
        pid_rf_ = std::make_unique<controllers::PD>(vals.at("k_p"), vals.at("k_d"), -max_diff_, max_diff_);
        //pid_rf_ = std::make_unique<controllers::PID_AW>(vals.at("k_p"), vals.at("k_d"), vals.at("k_i"), vals.at("k_aw"), -max_diff_, max_diff_);
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
        std::cout << "\n\n---------------\n";
        rpicomponents::mpu_angles angles;
        sensorics_->getKalmanAngles(angles);
        std::cout << "beta_s: " << beta_s_ << "\tis: " << angles.beta << "\terror: " << beta_s_ - angles.beta << std::endl;
        std::cout << "gamma_s: " << gamma_s_ << "\tis: " << angles.gamma << "\terror: " << gamma_s_ - angles.gamma << std::endl;
        int lb =  pid_lb_->calculate(-angles.beta, -beta_s_);
        int rb =  pid_rb_->calculate(angles.beta, beta_s_);
        int rf =  pid_rf_->calculate(angles.beta, beta_s_);
        int lf =  pid_lf_->calculate(-angles.beta, -beta_s_);
        std::cout << "lb: " << lb << "\trb: " << rb << "\trf: " << rf << "\tlf: " << lf <<  std::endl;
        lf_->SetOutputSpeed(throttle_ + lf);
        rf_->SetOutputSpeed(throttle_ + rf);
        lb_->SetOutputSpeed(throttle_ + lb);
        rb_->SetOutputSpeed(throttle_ + rb);
        std::cout << "---------------\n\n";
    }
    
    void Controls::startEsc(const std::unique_ptr<rpicomponents::Esc>& esc) 
    {
        //esc->Calibrate(false);
        esc->Arm();
        esc->SetOutputSpeed(0);
        utils::Waiter::SleepMillis(100);
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
}