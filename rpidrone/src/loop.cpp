#include "loop.hpp"
#include <iostream>
#include <unistd.h>
#include <fstream> 
#include "easylogging++.h"
//#include <filesystem>

namespace drone
{
    Loop::Loop(const std::string& config_path) 
    {
        loadConfig(config_path);
        pin::initGPIOs();
        server_ = std::make_unique<rpisocket::WiFiServer>(config_.at("server").at("port"), config_.at("server").at("bytes"), config_.at("server").at("delimiter"));
        on_btn_ = std::make_unique<rpicomponents::Button>(config_.at("misc").at("on_btn"));
        on_led_ = std::make_unique<rpicomponents::Led>(config_.at("misc").at("on_led"), pin::DIGITAL_MODE, pin::DIGITAL_MODE_MAX_VAL);
        status_led_ = std::make_unique<rpicomponents::Led>(config_.at("misc").at("status_led"), pin::DIGITAL_MODE, pin::DIGITAL_MODE_MAX_VAL);
        subq_ = std::make_shared<drone::SubscriberQueue<std::string>>();
        controls_ = std::make_unique<Controls>(config_.at("controls"), config_.at("sensorics"));
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
        // connect to app
        LOG(INFO) << "waiting for connection on " << server_->getServerIp() << ":" << server_->getPort() << std::endl;
        server_->connect();
        LOG(INFO) << "connected to " << server_->getConnectedClient() << std::endl;
        // while we are connected run read thread
        server_->readThreadOn();
        while(server_->hasConnection()){
            try{
                if(subq_->has_item()){
                    subq_->pop(read);
                    processInput(read);
                }
                controls_->control();
                server_->writeBytes("");
            } catch(const std::exception &exc) {
                //connection lost!
                LOG(ERROR) << exc.what() << std::endl;
                break;
            }
        }
        server_->readThreadOff(); //not connected anymore; stop thread
        subq_->clear();
    }
    
    void Loop::awaitBtnPress() 
    {
        while(!on_btn_->IsPressed()) {
            usleep(25000);
        }
    }

    void Loop::startupDrone() {
        awaitBtnPress();
        on_led_->TurnOn();
        controls_->startMotors(true);
        status_led_->TurnOn();
    }

    void Loop::loadConfig(const std::string& file) {
        std::ifstream ifs(file);
        config_ = json::parse(ifs);
    }
    
    void Loop::processInput(const std::string& read)
    {
        json j;
        std::string r = read;
        try{
            size_t pos =  r.find_first_of("{");
            r.erase(0, pos);
            j = json::parse(r);
            LOG(DEBUG) << "read: " << j << std::endl;
            controls_->process_input(j);
        } catch(const std::exception &exc){
            //we return old json
            LOG(ERROR) << exc.what() << std::endl;
        }
        
    }
}