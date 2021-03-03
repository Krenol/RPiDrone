#include "loop.hpp"
#include <unistd.h>
#include <fstream>
#include <future>
#include "easylogging++.h"
#include "input_parser.hpp"

#define NETWORK_LOG(LEVEL) CLOG(LEVEL, "network") //define network log
#define CONTROL_LOG(LEVEL) CLOG(LEVEL, "controls") //define control log
#if defined(EXEC_TIME_LOG)
#define EXEC_LOG(LEVEL) CLOG(LEVEL, "exec") //define exec time log
#endif


namespace drone
{
    Loop::Loop(const std::string& config_path)
    {
        loadConfig(config_path);
        pin::initGPIOs();
        connection_ = std::make_unique<Connection>(config_.server.port, config_.server.bytes, config_.server.delimiter);
        on_led_ = std::make_unique<rpicomponents::Led>(config_.leds.on_led, pin::DIGITAL_MODE, pin::DIGITAL_MODE_MAX_VAL);
        status_led_ = std::make_unique<rpicomponents::Led>(config_.leds.status_led, pin::DIGITAL_MODE, pin::DIGITAL_MODE_MAX_VAL);
        //readq_ = std::make_unique<drone::SubscriberQueue<std::string>>(config_.queues.read_size);
        controls_ = std::make_unique<Controls>(config_.controls, config_.sensors);
        server_q_ = std::make_shared<ServerSubscriberQueue>(config_.controls, config_.queues.read_size);
        tpe_ = std::make_unique<design_patterns::ThreadPoolExecutor>(1, config_.queues.write_size);
        connection_->subscribe(server_q_);
        #if defined(EXEC_TIME_LOG)
        EXEC_LOG(DEBUG) << "datetime;level;t_exec";
        #endif
    }


    Loop::~Loop() 
    {
        on_led_->TurnOff();
        status_led_->TurnOff();
        connection_->stopThread();
        pin::terminateGPIOs();
    }

    
    
    void Loop::loop() 
    {
        std::string delimiter = config_.server.delimiter;
        control_values vals;
        ControlValues cvs;
        connection_ -> startThread();
        CONTROL_LOG(INFO) << "starting main control loop";
        #if defined(EXEC_TIME_LOG)
        std::chrono::steady_clock::time_point last_call = std::chrono::steady_clock::now(), now;
        #endif
        while(1){
            try{
                if(server_q_->has_item()){
                    server_q_->pop(cvs);
                }
                controls_->control(vals);

                //controls_->getDroneCoordinates(c, 20); 
                
                tpe_->enqueue([this, vals, &delimiter](){
                    rpicomponents::GPSCoordinates c;
                    json j;
                    createOutputJson(vals, c, j);
                    if(connection_->hasConnection()) {
                        std::string msg = j.dump();
                        #if defined(NETWORK_DEBUG_LOGS)
                        NETWORK_LOG(DEBUG) << "writing " << msg;
                        #endif
                        connection_->writeMsg(msg + delimiter);
                    }
                });
                usleep(config_.logic.main_sleep); 
            } catch(const std::exception &exc) {
                CONTROL_LOG(ERROR) << exc.what();
            }
            #if defined(EXEC_TIME_LOG)
            now = std::chrono::steady_clock::now();
            EXEC_LOG(INFO) << std::chrono::duration_cast<std::chrono::milliseconds>(now - last_call).count();
            last_call = now;
            #endif
        }
    }

    void Loop::createOutputJson(const control_values& vals, const rpicomponents::GPSCoordinates& c, json& j) {
        j = {
                {"angles", {
                    {"yaw", vals.yaw_angle}, 
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
    


    void Loop::loadConfig(const std::string& file) {
        std::ifstream ifs(file);
        config_ = json::parse(ifs);
    }
    
    void Loop::processInput(const std::string& read)
    {
        json j;
        try{
            j = json::parse(read);
            #if defined(NETWORK_DEBUG_LOGS)
            NETWORK_LOG(DEBUG) << "parsed input to json " << j;
            #endif
            from_json(j, last_input_);
            controls_->process_input(last_input_);
        } catch(const std::exception &exc){
            //we return old json
            NETWORK_LOG(ERROR) << exc.what() << "read string: " << read;
        }
        
    }
}