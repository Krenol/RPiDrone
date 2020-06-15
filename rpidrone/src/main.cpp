//#include "rpicomponents/rpicomponents.hpp"
#include "rpisocket/rpisocket.hpp"
#include "design_patterns/design_patterns.hpp"
#include <iostream>
#include <string>
#include "sensorics.hpp"
#include <future>
#include <chrono>
#include <cstdlib>

class SubQ : public design_patterns::SubscriberQueue<std::string> {
    public:
        SubQ() : SubscriberQueue(1) {

        }

    std::mutex mtx_; 
    void process_next(const std::string& msg) {
        std::lock_guard<std::mutex> guard(mtx_);
        std::cout << "rcvd msg: " << msg << std::endl;
    }
};


int getPort(){
    auto env_var = std::getenv("SOCKET_PORT");
    int port = 8888;
    try {
        if(env_var){
            port = std::stoi(env_var); 
        } 
    } catch(...){}
    
    return port;
}


int main() {
    drone::Sensorics sensorics(rpicomponents::UNIT_M);
    rpisocket::WiFiServer server(getPort());
    std::string data;
    auto s = std::make_shared<SubQ>();
    server.subscribe(s);
    while(1){
        std::cout << "waiting for connection..." << std::endl;
        server.connect();
        std::cout << "connected to " << server.getConnectedClient() << std::endl;
        bool status = 1;
        while(server.hasConnection() && status){
            try{
                auto dist = std::async(&drone::Sensorics::getDistances, &sensorics);
                data = "{\"distLeft\":" + std::to_string(dist.get().dist_front) + "}";
                status = server.writeBytes(data);
                std::cout << "msg: " << server.readBytes() << std::endl;
            } catch(...) {
                //connection lost!
                status = 0;
            }
        }
    }
    
    
    return 0;
}

