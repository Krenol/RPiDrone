#include "controls.hpp"
#include <future>
namespace drone
{
    Controls::Controls(const json& escs) : SubscriberQueue(1), idle_ {escs.at("idle")}
    {
        int esc_max = escs.at("max");
        int esc_min = escs.at("min");
        auto esc_pins = escs.at("pins");
        json val;
        std::string pos;
        for(auto esc : esc_pins.items()){
            val = esc.value();
            pos = val.at("pos");
            if(pos == "rb"){
                rb_ = std::make_unique<rpicomponents::Esc>(pin::GPIO_MAP.at(val.at("pin")), esc_min, esc_max);
            } else if(pos == "lb"){
                lb_ = std::make_unique<rpicomponents::Esc>(pin::GPIO_MAP.at(val.at("pin")), esc_min, esc_max);
            } else if(pos == "rf") {
                rf_ = std::make_unique<rpicomponents::Esc>(pin::GPIO_MAP.at(val.at("pin")), esc_min, esc_max);
            } else if (pos == "lf") {
                lf_ = std::make_unique<rpicomponents::Esc>(pin::GPIO_MAP.at(val.at("pin")), esc_min, esc_max);
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
    
    void Controls::process_next(const std::string& input) 
    {
        json j;
        try{
            j = json::parse(input); 
        } catch(...) {
            return; // on failure we parse next
        }
        const std::lock_guard<std::mutex> lock(mtx_);
        throttle_ = j.at("throttle");
        lf_->SetOutputSpeed(throttle_);
        rf_->SetOutputSpeed(throttle_);
        lb_->SetOutputSpeed(throttle_);
        rb_->SetOutputSpeed(throttle_);
    }
    
    int Controls::getThrottle() 
    {
        return throttle_;
    }
    
    void Controls::startEsc(const std::unique_ptr<rpicomponents::Esc>& esc) 
    {
        esc->Calibrate(false);
        esc->SetOutputSpeed(idle_);
        utils::Waiter::SleepMillis(100);
    }
}