#include "controller.hpp"

namespace controllers
{
    template<typename T>
    void Controller<T>::setDt() 
    {
        auto now = std::chrono::steady_clock::now();
        dt_ = std::chrono::duration_cast<T>(now - last_call_).count();
        last_call_ = now;
    }
    
    template<typename T>
    Controller<T>::Controller(const Eigen::MatrixXd& p, const Eigen::MatrixXd& d, const Eigen::MatrixXd& i, const Eigen::VectorXd& min, const Eigen::VectorXd& max, bool bounded) :
        kp_{p.transpose()}, ki_ {i.transpose()}, kd_ {d.transpose()}, min_ {min}, max_ {max}, bounded_ {bounded}
    {
        if(bounded_) {
            if(max_.size() != min_.size()){
                throw std::invalid_argument("max and min vectors must have the same length!");
            }
            auto diffs = max_ - min_;
            for(int i = 0; i< diffs.size(); i++) {
                if(diffs[i] < 0) throw std::invalid_argument("max values must be greater than min values!");
            }
        }
        first_call_ = true;
        integral_ = Eigen::VectorXd::Zero(p.rows());
        last_err_ = Eigen::VectorXd::Zero(p.rows());
    }
    
    template<typename T>
    void Controller<T>::bound(Eigen::VectorXd& values) 
    {
        if(bounded_){
            for(int i = 0; i < values.size(); i++) {
                values[i] = values[i] < min_[i] ? min_[i] : (values[i] > max_[i] ? max_[i] : values[i]);
            }
        }
    }
    
    template<typename T>
    void Controller<T>::calculateP(const Eigen::VectorXd& err, Eigen::VectorXd& out) 
    {
        out = kp_ * err;
    }
    
    template<typename T>
    void Controller<T>::calculateD(const Eigen::VectorXd& err, Eigen::VectorXd& out) 
    {
        if(dt_ > 0) {
            out = kd_ * (err - last_err_) / dt_;
        } else {
            out = kd_ * err;
        }
        last_err_ = err;
    }
    
    template<typename T>
    void Controller<T>::calculateI(const Eigen::VectorXd& err, Eigen::VectorXd& out) 
    {   
        integral_ = integral_ + err * dt_;
        out = ki_ * integral_;
    }
    
    template<typename T>
    Controller<T>::Controller(const Eigen::MatrixXd& p, const Eigen::MatrixXd& d, const Eigen::MatrixXd& i) : Controller(p, d, i, Eigen::VectorXd::Zero(1), Eigen::VectorXd::Zero(1), false)
    {
        
    }
    
    template<typename T>
    Controller<T>::Controller(const Eigen::MatrixXd& p, const Eigen::MatrixXd& d, const Eigen::MatrixXd& i, const Eigen::VectorXd& min, const Eigen::VectorXd& max) : Controller(p, d, i, min, max, true)
    {
        
    }
    
    template<typename T>
    void Controller<T>::calculate(const Eigen::VectorXd& x_cur, const Eigen::VectorXd& x_desired, Eigen::VectorXd& out) 
    {
        if(first_call_){
            last_call_ = std::chrono::steady_clock::now();
            first_call_ = false;
        }
        auto err = x_cur - x_desired;
        std::lock_guard<std::mutex> guard(mtx_);
        setDt();
        control(err, out);
        bound(out);
    }

    template class Controller<std::chrono::hours>;
    template class Controller<std::chrono::minutes>;
    template class Controller<std::chrono::seconds>;
    template class Controller<std::chrono::milliseconds>;
    template class Controller<std::chrono::microseconds>;
    template class Controller<std::chrono::nanoseconds>;
}