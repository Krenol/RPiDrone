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
        std::lock_guard<std::mutex> guard(mtx_);
        queue_.push(item);
    }
    
    template<class T>
    T Queue<T>::pop() 
    {
        std::lock_guard<std::mutex> guard(mtx_);
        T val;
        intern_pop(val);
        return val;
    }
    
    template<class T>
    T Queue<T>::pop_or_last() 
    {
        std::lock_guard<std::mutex> guard(mtx_);
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
        std::lock_guard<std::mutex> guard(mtx_);
        if(queue_.empty()){
            item = last_value_;
        } else {
            intern_pop(item);
        }
    }
    
    
    template<class T>
    void Queue<T>::pop(T& item) 
    {
        std::lock_guard<std::mutex> guard(mtx_);
        intern_pop(item);
    }
    
    template<class T>
    void Queue<T>::push(T&& item) 
    {
        std::lock_guard<std::mutex> guard(mtx_);
        queue_.push(std::move(item));
    }
}