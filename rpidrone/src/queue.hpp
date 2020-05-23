#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>

/*see https://github.com/juanchopanza/cppblog/blob/master/Concurrency/Queue/Queue.h */
#ifndef RPIDRONE_QUEUE_H
#define RPIDRONE_QUEUE_H

namespace rpidrone
{
    template <class T>
    class Queue {
    public:
        Queue() = default;
        Queue(const Queue&) = delete;            // disable copying
        Queue& operator=(const Queue&) = delete; // disable assignment
        void push(const T& item);
        void push(T&& item);
        T pop();
        void pop(T& item);
    
    private:
        std::queue<T> queue_;
        std::mutex mutex_;
        std::condition_variable cond_;
        
    };
}
#endif