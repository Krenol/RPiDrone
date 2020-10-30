#include "rpisocket/rpisocket.hpp"
#include "design_patterns/design_patterns.hpp"
#include <iostream>
#include <string>
#include "sensorics.hpp"
#include <future>
#include <chrono>
#include <cstdlib>
#include "controls.hpp"
#include <fstream> 
#include <filesystem>

using json = nlohmann::json;


int getPort(){
    auto env_var = std::getenv("SOCKET_PORT");
    int port = -1;
    try {
        if(env_var){
            port = std::stoi(env_var); 
        } 
    } catch(...){}
    
    return port;
}

void loadConfig(const std::string& file, json& j) {
    std::ifstream ifs(file);
    j = json::parse(ifs);
}

int main() {
    //start pigpio
    pin::initGPIOs();
    // prepare all objects and data
    json config;
    loadConfig("/home/pi/mnt/RPiDrone/config.json", config);
    drone::Sensorics sensorics(rpicomponents::UNIT_M, config.at("mpu"));
    auto controls = std::make_shared<drone::Controls>(config.at("escs"));
    rpisocket::WiFiServer server(getPort());
    std::string data;

    // start the motors and subscribe the controls to the server
    controls->startMotors();
    server.subscribe(controls);

    while(1){
        // connect to app
        std::cout << "waiting for connection..." << std::endl;
        server.connect();
        std::cout << "connected to " << server.getConnectedClient() << std::endl;
        bool status = 1;
        // while we are connected run read thread
        server.readThreadOn();
        while(server.hasConnection() && status){
            try{
                auto dist = std::async(&drone::Sensorics::getDistances, &sensorics);
                data = "{\"distLeft\":" + std::to_string(dist.get().dist_front) + "}";
                status = server.writeBytes(data);
            } catch(...) {
                //connection lost!
                status = 0;
            }
        }
        server.readThreadOff(); //not connected anymore; stop thread
    }
    pin::terminateGPIOs();
    return 0;
}

