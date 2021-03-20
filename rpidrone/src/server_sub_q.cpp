#include "server_sub_q.hpp"
#include <nlohmann/json.hpp>
#include "misc.hpp"
#include "parsers/json_input_parser.hpp"

#define NETWORK_LOG(LEVEL) CLOG(LEVEL, "network") //define network log

using json = nlohmann::json;

namespace drone
{

    ServerSubscriberQueue::ServerSubscriberQueue(const ControlsStruct &controls, int max_q_size, bool motors_off_on_disconnect) : design_patterns::SubscriberQueue<std::string>(1), controls_{controls}, max_size_{max_q_size}, motors_off_{motors_off_on_disconnect}
    {
    }

    void ServerSubscriberQueue::process_next(const std::string &msg)
    {
        json j;
        UserInput c;
        try
        {
            j = json::parse(msg);
            #if defined(NETWORK_DEBUG_LOGS)
            NETWORK_LOG(DEBUG) << "parsed input to json " << j;
            #endif
            if (JSON_EXISTS(j, "disconnected") && j.at("disconnected") == true)
            {
                if(motors_off_) {
                    c.throttle = 0;
                } else {
                    c.throttle = controls_.escs.idle;
                }
            }
            else
            {
                Input in;
                from_json(j, in);
                c = UserInput(in, controls_);
            }
            update(c);
        }
        catch (const std::exception &exc)
        {
            NETWORK_LOG(ERROR) << exc.what() << "read string: " << msg;
        }
    }

    void ServerSubscriberQueue::update(const UserInput &msg)
    {
        if (item_count() > max_size_)
        {
            clear();
        }
        push(msg);
    }

}