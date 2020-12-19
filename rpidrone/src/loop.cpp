#include "loop.hpp"
#include <iostream>
#include <unistd.h>
#include <fstream>
#include "easylogging++.h"

#define NETWORK_LOG(LEVEL) CLOG(LEVEL, "network") //define network log
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
        readq_ = std::make_unique<drone::SubscriberQueue<std::string>>();
        writeq_ = std::make_unique<drone::SubscriberQueue<std::string>>();
        controls_ = std::make_unique<Controls>(config_.at("controls"), config_.at("sensorics"));
    }


    Loop::~Loop() 
    {
        server_->readThreadOff();
        on_led_->TurnOff();
        status_led_->TurnOff();
        thread_on_ = false;
        conn_thread_.join();
        pin::terminateGPIOs();
    }

    void Loop::connectionHandler() {
        thread_on_ = true;
        std::string msg, buf, delimiter = config_.at("server").at("delimiter");
        while(1) {
            NETWORK_LOG(INFO) << "waiting for connection on " << server_->getServerIp() << ":" << server_->getPort();
            server_->connect();
            NETWORK_LOG(INFO) << "connected to " << server_->getConnectedClient();
            while(server_->hasConnection() && thread_on_){
                try{
                    server_->readBytes(msg);
                    buf += msg;
                    processServerRead(buf, delimiter);
                    if(writeq_->has_item()){
                        writeq_->pop(msg);
                        server_->writeBytes(msg);
                    }
                } catch(...) {
                    // on error stop
                    break;
                }
            }   
            readq_->clear();
            writeq_->clear();
            if(thread_on_) {
                readq_->update("{\"joystick\":{\"degrees\":0,\"offset\":0}}");
            } else {
                server_->disconnect();
                return;
            }
        }
    }

    void Loop::processServerRead(std::string& buf, const std::string& delimiter) {
        std::string msg;
        std::size_t pos, pos1;
        while ((pos = buf.find(delimiter)) != std::string::npos) {
            msg = buf.substr(0, pos);
            pos1 = msg.find_first_of("{");
            msg.erase(0, pos1);
            readq_->update(msg);
            buf.erase(0, pos + delimiter.length());
        }
    }
    
    void Loop::loop() 
    {
        std::string read;
        control_values vals;
        json j;
        conn_thread_ = std::thread(&Loop::connectionHandler, this);
        // connect to app
        while(1){
            if(readq_->has_item()){
                readq_->pop(read);
                processInput(read);
            }
            controls_->control(vals);
            createOutputJson(vals, j);
            writeq_->update(j.dump());

            usleep(10000); // sleep 10ms
        }
    }

    void Loop::createOutputJson(control_values& vals, json& j) {
        j = {"angles", {{"yaw", 0}, {"pitch", vals.pitch_angle}, {"roll", vals.roll_angle}},
             "position": {}
            };
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
        controls_->startMotors();
        status_led_->TurnOn();
    }
    
    bool Loop::hasConnection() 
    {
        return server_->hasConnection();
    }

    void Loop::loadConfig(const std::string& file) {
        std::ifstream ifs(file);
        config_ = json::parse(ifs);
    }
    
    void Loop::processInput(const std::string& read)
    {
        json j;
        try{
            j = json::parse(read);
            NETWORK_LOG(INFO) << "parsed input to json " << j;
            controls_->process_input(j);
        } catch(const std::exception &exc){
            //we return old json
            NETWORK_LOG(ERROR) << exc.what() << "read string: " << read << std::endl;
        }
        
    }

    void Loop::calibrate() {
        controls_->calibrateSensors();
    }

    void Loop::saveCalibration(const std::string& path) {
        controls_->saveCalibration(path);
    }

    void Loop::loadCalibration(const std::string& path) {
        controls_->loadCalibration(path);
    }
}