#include "pid_aw.hpp"

namespace controllers
{
    template<typename T>
    void PID_AW<T>::control(const Eigen::VectorXd& err, Eigen::VectorXd& out) 
    {
        Eigen::VectorXd data;
        Controller<T>::calculateP(err, out);
        Controller<T>::calculateI(err, data);
        out = out + data;
        Controller<T>::calculateD(err, data);
        out = out + data;
        //anti windup part
        data = out;
        Controller<T>::bound(out);
        auto fac = (p_aw_ * (out - data)).mean();
        for(auto i = 0; i < Controller<T>::integral_.size(); i++) {
            Controller<T>::integral_[i] += fac;
        }
    }
    
    template<typename T>
    PID_AW<T>::PID_AW(const Eigen::MatrixXd& p, const Eigen::MatrixXd& d, const Eigen::MatrixXd& i, const Eigen::VectorXd& p_aw, const Eigen::VectorXd& min, const Eigen::VectorXd& max) : Controller<T>(p, d, i, min, max), p_aw_{p_aw}
    {
        
    }

    template class PID_AW<std::chrono::hours>;
    template class PID_AW<std::chrono::minutes>;
    template class PID_AW<std::chrono::seconds>;
    template class PID_AW<std::chrono::milliseconds>;
    template class PID_AW<std::chrono::microseconds>;
    template class PID_AW<std::chrono::nanoseconds>;
}