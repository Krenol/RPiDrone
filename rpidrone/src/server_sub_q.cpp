#include "server_sub_q.hpp"
#include "input_parser.hpp"
#include "easylogging++.h"
#include <nlohmann/json.hpp>

#define NETWORK_LOG(LEVEL) CLOG(LEVEL, "network") //define network log

using json = nlohmann::json;

namespace drone
{

    ServerSubscriberQueue::ServerSubscriberQueue(const ControlsStruct &controls, int max_q_size) : design_patterns::SubscriberQueue<std::string>(1), controls_{controls}, max_size_{max_q_size}
    {
    }

    void ServerSubscriberQueue::process_next(const std::string &msg)
    {
        json j;

        try
        {
            j = json::parse(msg);
            #if defined(NETWORK_DEBUG_LOGS)
            NETWORK_LOG(DEBUG) << "parsed input to json " << j;
            #endif
            Input in;
            from_json(j, in);
            ControlValues c(in, controls_);
            update(c);
        }
        catch (const std::exception &exc)
        {
            //we return old json
            NETWORK_LOG(ERROR) << exc.what() << "read string: " << msg;
        }
    }

    void ServerSubscriberQueue::update(const ControlValues &msg)
    {
        if (item_count() > max_size_)
        {
            clear();
        }
        push(msg);
    }

}