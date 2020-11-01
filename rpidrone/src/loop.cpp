#include "loop.hpp"
#include <iostream>
#include <unistd.h>
#include <fstream> 
//#include <filesystem>

namespace drone
{
    Loop::Loop(const std::string& config_path) 
    {
        loadConfig(config_path);
        pin::initGPIOs();
        server_ = std::make_unique<rpisocket::WiFiServer>(config_.at("server").at("port"), config_.at("server").at("bytes"));
        on_btn_ = std::make_unique<rpicomponents::Button>(config_.at("misc").at("on_btn"));
        on_led_ = std::make_unique<rpicomponents::Led>(config_.at("misc").at("on_led"), pin::DIGITAL_MODE, pin::DIGITAL_MODE_MAX_VAL);
        status_led_ = std::make_unique<rpicomponents::Led>(config_.at("misc").at("status_led"), pin::DIGITAL_MODE, pin::DIGITAL_MODE_MAX_VAL);
        subq_ = std::make_shared<drone::SubscriberQueue<std::string>>();
        controls_ = std::make_unique<Controls>(config_.at("controls"));
        sensorics_ = std::make_unique<Sensorics>(config_.at("sensorics"));
        server_->subscribe(subq_);
    }


    Loop::~Loop() 
    {
        server_->readThreadOff();
        on_led_->TurnOff();
        status_led_->TurnOff();
        pin::terminateGPIOs();
    }
    
    void Loop::loop() 
    {
        std::string read, write;
        json j;
        rpicomponents::mpu_angles angles;
        // connect to app
        std::cout << "waiting for connection..." << std::endl;
        server_->connect();
        std::cout << "connected to " << server_->getConnectedClient() << std::endl;
        // while we are connected run read thread
        server_->readThreadOn();
        while(server_->hasConnection()){
            try{
                if(subq_->has_item()){
                    subq_->pop(read);
                    processInput(read, j);
                    std::cout << "received msg: " << read << std::endl;
                }
                sensorics_->getKalmanAngles(angles);
                controls_->control(angles);
            } catch(...) {
                //connection lost!
                break;
            }
        }
        server_->readThreadOff(); //not connected anymore; stop thread
    }
    
    void Loop::awaitBtnPress() 
    {
        while(!on_btn_->IsPressed()) {
            usleep(50000);
        }
    }

    void Loop::startupDrone() {
        awaitBtnPress();
        on_led_->TurnOn();
        controls_->startMotors();
        status_led_->TurnOn();
    }

    json Loop::loadConfig(const std::string& file) {
        std::ifstream ifs(file);
        config_ = json::parse(ifs);
    }
    
    void Loop::processInput(const std::string& read, json& j) 
    {
        json old = j;
        try{
            j = json::parse(read);
            controls_->process_input(j);
        } catch(...){
            //we return old json
            j = old;
        }
    }
}