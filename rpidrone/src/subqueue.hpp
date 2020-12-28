#include "design_patterns/design_patterns.hpp"

#ifndef DRONE_SUBSCRIBER_QUEUE_H
#define DRONE_SUBSCRIBER_QUEUE_H

namespace drone
{
    //adapted from https://codereview.stackexchange.com/questions/144545/c-14-thread-pool-executor-design
    template <class T>
    class SubscriberQueue : public design_patterns::Subscriber<T>, public design_patterns::Queue<T> {
    private:
        const int max_size_ {100};
    public:

        SubscriberQueue(int max_size = 100) : max_size_{max_size}
        {

        }

        /*
        Method that is colled by publisher once a msg is received
        @param msg: The received message
        */
        void update(const T& msg) {
            if(design_patterns::Queue<T>::item_count() > max_size_){
                clear();
            }
            design_patterns::Queue<T>::push(msg);
        }

        void clear() {
            design_patterns::Queue<T>::clear();
        }
    
    };
}

#endif