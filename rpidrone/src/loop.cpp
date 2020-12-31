#include "loop.hpp"
#include <unistd.h>
#include <fstream>
#include <future>
#include "easylogging++.h"

#define NETWORK_LOG(LEVEL) CLOG(LEVEL, "network") //define network log
#define CONTROL_LOG(LEVEL) CLOG(LEVEL, "controls") //define control log
//#include <filesystem>

namespace drone
{
    Loop::Loop(const std::string& config_path)
    {
        loadConfig(config_path);
        pin::initGPIOs();
        server_ = std::make_unique<rpisocket::WiFiServer>(config_.server.port, config_.server.bytes);
        on_led_ = std::make_unique<rpicomponents::Led>(config_.leds.on_led, pin::DIGITAL_MODE, pin::DIGITAL_MODE_MAX_VAL);
        status_led_ = std::make_unique<rpicomponents::Led>(config_.leds.status_led, pin::DIGITAL_MODE, pin::DIGITAL_MODE_MAX_VAL);
        readq_ = std::make_unique<drone::SubscriberQueue<std::string>>(config_.queues.read_size);
        controls_ = std::make_unique<Controls>(config_.controls, config_.sensors);
        tpe_ = std::make_unique<design_patterns::ThreadPoolExecutor>(1, config_.queues.write_size);
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
        std::string msg, buf, delimiter = config_.server.delimiter;
        int max_buf_size = buf.max_size() * 3 / 4;
        while(1) {
            NETWORK_LOG(INFO) << "waiting for connection on " << server_->getServerIp() << ":" << server_->getPort();
            server_->connect();
            buf = "";
            NETWORK_LOG(INFO) << "connected to " << server_->getConnectedClient();
            while(server_->hasConnection() && thread_on_){
                if(buf.size() > max_buf_size) {
                    NETWORK_LOG(WARNING) << "socket read buffer about to overflow! dumping buffer";
                    buf.clear();
                }
                try{
                    server_->readBytes(msg);
                    buf += msg;
                    processServerRead(buf, delimiter);
                } catch(const std::exception &exc) {
                    NETWORK_LOG(ERROR) << exc.what();
                    break;
                }
            }   
            readq_->clear();
            tpe_->clear();
            if(thread_on_) {  
                readq_->update("{\"joystick\":{\"degrees\":0,\"offset\":0}}");           
                if(config_.logic.motors_off_disconnect) {
                    usleep(config_.logic.sleep_disconnect);
                    controls_->motorsOff();
                } 
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
        std::string read, delimiter = config_.server.delimiter;
        control_values vals;
        rpicomponents::GPSCoordinates c;
        json j;
        conn_thread_ = std::thread(&Loop::connectionHandler, this);
        CONTROL_LOG(INFO) << "starting main control loop";
        while(1){
            try{
                if(readq_->has_item()){
                    readq_->pop(read);
                    processInput(read);
                }
                controls_->control(vals);

                //controls_->getDroneCoordinates(c, 20); 
                createOutputJson(vals, c, j);
                tpe_->enqueue([this, j, &delimiter](){
                    if(server_->hasConnection()) {
                        std::string msg = j.dump();
                        NETWORK_LOG(DEBUG) << "writing " << msg;
                        server_->writeBytes(msg + delimiter);
                    }
                });
                usleep(config_.logic.main_sleep); 
            } catch(const std::exception &exc) {
                CONTROL_LOG(ERROR) << exc.what();
            }
            
        }
    }

    void Loop::createOutputJson(const control_values& vals, const rpicomponents::GPSCoordinates& c, json& j) {
        j = {
                {"angles", {
                    {"yaw", 0}, 
                    {"pitch", vals.pitch_angle}, 
                    {"roll", vals.roll_angle}}
                },
                {"position", {
                    {"latitude", c.latitude}, 
                    {"longitude", c.longitude}, 
                    {"altitude", c.altitude}}
                },
                { "sensors",{
                    {"barometric_height", controls_->getAltitude()}}
                }
            };
    }
    
    void Loop::startupDrone() {
        on_led_->TurnOn();
        sleep(5); //give 5s to get away from drone
        controls_->startMotors();
        sleep(5); //give motors 5s
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
            NETWORK_LOG(DEBUG) << "parsed input to json " << j;
            from_json(j, last_input_);
            controls_->process_input(last_input_);
        } catch(const std::exception &exc){
            //we return old json
            NETWORK_LOG(ERROR) << exc.what() << "read string: " << read;
        }
        
    }
}