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

        /*
        Method to push back a value to the queue
        @param item: The item to be pushed back
        */
        void push(const T& item);
        /*
        Method to push back a value to the queue
        @param item: The item to be pushed back
        */
        void push(T&& item);

        /*
        Method to pop the first item in the queue
        @returns the first item in the queue
        */
        T pop();

        /*
        Method to pop the first item in the queue
        @param item: Item to hold popped value
        */
        void pop(T& item);

        /*
        Method to pop the first item in the queue or the last popped value
        if queue is empty
        @returns the first item in the queue or the last popped item
        */
        T pop_or_last();

        /*
        Method to pop the first item in the queue or the last popped value
        if queue is empty
        @param item: Item to hold popped value / last value
        */
        void pop_or_last(T& item);
    
    private:
        /*
        Method to reduce amount of duplciate code for popping
        @param item: Item to hold popped value
        */
        void intern_pop(T& item);
        
        T last_value_;
        std::queue<T> queue_;
        std::mutex mutex_;
        std::condition_variable cond_;
        
    };
}
#endif