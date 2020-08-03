#include "sensorics.hpp"
#include "utils/utils.hpp"


namespace drone
{
    Sensorics::Sensorics() : Sensorics::Sensorics(rpicomponents::UNIT_MM)
    {
        
    }
    
    Sensorics::Sensorics(rpicomponents::DISTANCE_UNIT unit) : unit_{unit}
    {
        // auto trigger_pin = pin::GET_PIN_ID(std::getenv("TRIGGER_PIN"));  
        // std::shared_ptr<pin::Pin> trigger = std::move(pin::PinCreator::CreateDigitalPin(trigger_pin, 1));
        // auto echo_pin_front = pin::GET_PIN_ID(std::getenv("ECHO_PIN_FRONT"));
        // std::shared_ptr<pin::Pin> echo_front = std::move(pin::PinCreator::CreateInputPin(echo_pin_front, 1));
        // uss_front_ = std::make_unique<rpicomponents::UltrasonicSensor>(trigger, echo_front);
        auto addr = std::stoi(utils::Environment::getVar("MPU_ADDR", "0x68"));
        mpu_ = std::make_unique<rpicomponents::MPU6050>(addr, rpicomponents::G_4, rpicomponents::DPS_500);
    }

    Distances Sensorics::getDistances() const
    {
        Distances dist;
        // dist.dist_front = uss_front_->MeasureDistance(rpicomponents::UNIT_M);
        return dist;
    }

    bool Sensorics::calibrate() 
    {
        
    }
    
    bool Sensorics::loadCalibration(const std::string& path) 
    {
        
    }
    
    bool Sensorics::storeCalibration(const std::string& path) 
    {
        
    }
}