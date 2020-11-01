#include "controls.hpp"
#include <future>
namespace drone
{
    Controls::Controls(const json& controls) : idle_ {controls.at("escs").at("idle")}, esc_max_ {controls.at("escs").at("max")}, esc_min_ {controls.at("escs").at("min")}
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
    
    void Controls::startMotors() 
    {
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
        throttle_ =  (throttle_ * (esc_max_ - idle_) / 100) + idle_;
        lf_->SetOutputSpeed(throttle_);
        rf_->SetOutputSpeed(throttle_);
        lb_->SetOutputSpeed(throttle_);
        rb_->SetOutputSpeed(throttle_);
    }
    
    int Controls::getThrottle() 
    {
        return throttle_;
    }
    
    void Controls::control(const rpicomponents::mpu_angles& angles) 
    {
        
    }
    
    void Controls::startEsc(const std::unique_ptr<rpicomponents::Esc>& esc) 
    {
        esc->Calibrate(false);
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
}