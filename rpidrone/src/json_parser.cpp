#include "json_parser.hpp"

namespace drone
{
    void to_json(nlohmann::json &j, const Config &cfg) 
    {
        
    }
    
    void from_json(const nlohmann::json &j, Config &cfg) 
    {
        cfg.leds = Leds(j.at("leds").at("on_led"), j.at("leds").at("status_led"));
        cfg.logic = Logic(j.at("logic").at("main_loop_sleep"), j.at("logic").at("motors_off_on_disconnect"));
        cfg.server = Server(j.at("server").at("port"), j.at("server").at("bytes"), j.at("server").at("delimiter"));
        cfg.queues = Queues(j.at("queues").at("read_size"), j.at("queues").at("write_size"));
    }
}