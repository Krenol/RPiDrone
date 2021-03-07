#include "design_patterns/design_patterns.hpp"
#include "easylogging++.h"
#include "input_parser.hpp"

#define NETWORK_LOG(LEVEL) CLOG(LEVEL, "network") //define network log

#ifndef DRONE_SERVER_SUBSCRIBER_QUEUE_H
#define DRONE_SERVER_SUBSCRIBER_QUEUE_H

namespace drone
{
    class ServerSubscriberQueue : public design_patterns::SubscriberQueue<std::string>, public design_patterns::Queue<ControlValues> {
    private:
    const ControlsStruct controls_;
    const int max_size_ {100};

    public:

        ServerSubscriberQueue(const ControlsStruct &controls, int max_q_size = 100);

        
        void process_next(const std::string& msg);

        void update(const ControlValues& msg);
    
    };
}

#endif