#include "queue.hpp"

namespace rpidrone
{
    template<class T>
    void Queue<T>::intern_pop(T& item) 
    {
        item = queue_.front();
        last_value_ = item;
        queue_.pop();
    }

    template<class T>
    void Queue<T>::push(const T& item) 
    {
        std::unique_lock<std::mutex> mlock(mutex_);
        queue_.push(item);
        mlock.unlock();
        cond_.notify_one();
    }
    
    template<class T>
    T Queue<T>::pop() 
    {
        std::unique_lock<std::mutex> mlock(mutex_);
        T val;
        while (queue_.empty())
        {
            cond_.wait(mlock);
        }
        intern_pop(val);
        return val;
    }
    
    template<class T>
    T Queue<T>::pop_or_last() 
    {
        std::unique_lock<std::mutex> mlock(mutex_);
        T val;
        if(queue_.empty()){
            val = last_value_;
        } else {
            intern_pop(val);
        }
        return val;
    }

    template<class T>
    void Queue<T>::pop_or_last(T& item) 
    {
        std::unique_lock<std::mutex> mlock(mutex_);
        if(queue_.empty()){
            item = last_value_;
        } else {
            intern_pop(item);
        }
    }
    
    
    template<class T>
    void Queue<T>::pop(T& item) 
    {
        std::unique_lock<std::mutex> mlock(mutex_);
        
        while (queue_.empty())
        {
            cond_.wait(mlock);
        }

        intern_pop(item);
    }
    
    template<class T>
    void Queue<T>::push(T&& item) 
    {
        std::unique_lock<std::mutex> mlock(mutex_);
        queue_.push(std::move(item));
        mlock.unlock();
        cond_.notify_one();
    }
}