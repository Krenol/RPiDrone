#include <functional>
#include <vector>
#include <mutex>  
#include <memory>
#include <time.h>
#include <stdlib.h>
#include "subscriber.hpp"

#ifndef RPIDRONE_PUBLISHER_H
#define RPIDRONE_PUBLISHER_H

namespace rpidrone
{
    template <class T>
    class Publisher{
        public:

            Publisher() {
                //init seed for rand
                srand(time(NULL));
            }
            
            /* 
            Method to sub to this class

            @param sub: The subscriber
            @returns position of current subscriber
            */
            int subscribe(std::shared_ptr<rpidrone::Subscriber<T>> sub) {
                std::lock_guard<std::mutex> guard(mtx_);
                subs_.push_back(std::move(sub));
                return subs_.size() - 1;
            }

            /* 
            Method to unsub from this class

            @param pos: TThe postition of the sub as given by the sub function
            */
            bool unsubscribe(int pos) {
                std::lock_guard<std::mutex> guard(mtx_);
                if(pos >= subs_.size() || pos < 0) return false;
                subs_.erase (subs_.begin() + pos);
                return true;
            }

            /*
            Method to notify all subs to this object
            @param msg: The msg for the notification
            */
            void notifyAll(const T& msg) {
                std::lock_guard<std::mutex> guard(mtx_);
                for(auto i = 0; i < subs_.size(); i++) {
                    notifier(i, msg);
                }
            }

            /*
            Method to notify one sub. 
            Sub is chosen randomly!
            @param msg: The msg for the notification
            */
            void notify(const T& msg) {
                std::lock_guard<std::mutex> guard(mtx_);
                int pos = rand() % subs_.size(); 
                notifier(pos, msg);
            }

            /*
            Method to notify one sub. 

            @param pos: The sub to be notified
            @param msg: The msg for the notification
            */
            void notify(int pos, const T& msg) {
                std::lock_guard<std::mutex> guard(mtx_);
                notifier(pos, msg);
            }

        private:
            std::vector<std::shared_ptr<rpidrone::Subscriber<T>>> subs_;
            std::mutex mtx_;
            /* 
            Internal notifier
            @param pos: The sub to be notified
            @param msg: The msg for the notification
            */
            void notifier(int pos, const T& msg) {
                try{
                    (subs_.at(pos))->update(msg);
                } catch(...) {
                    //ignore for now
                }
            }

    };
}

#endif