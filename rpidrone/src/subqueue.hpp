#include "design_patterns/design_patterns.hpp"

#ifndef DRONE_SUBSCRIBER_QUEUE_H
#define DRONE_SUBSCRIBER_QUEUE_H

namespace drone
{
    //adapted from https://codereview.stackexchange.com/questions/144545/c-14-thread-pool-executor-design
    template <class T>
    class SubscriberQueue : public design_patterns::Subscriber<T>, public design_patterns::Queue<T> {

    public:

        /*
        Method that is colled by publisher once a msg is received
        @param msg: The received message
        */
        void update(const T& msg) {
            design_patterns::Queue<T>::push(msg);
        }
    
    };
}

#endif