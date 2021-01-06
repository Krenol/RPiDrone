#include "pid.hpp"

namespace controllers
{
    template<typename T>
    void PID<T>::control(const Eigen::VectorXd& err, Eigen::VectorXd& out) 
    {
        Eigen::VectorXd data;
        Controller<T>::calculateP(err, out);
        Controller<T>::calculateI(err, data);
        out = out + data;
        Controller<T>::calculateD(err, data);
        out = out + data;
    }
    
    template<typename T>
    PID<T>::PID(const Eigen::MatrixXd& p, const Eigen::MatrixXd& d, const Eigen::MatrixXd& i) : Controller<T>(p, d, i)
    {
        
    }
    
    template<typename T>
    PID<T>::PID(const Eigen::MatrixXd& p, const Eigen::MatrixXd& d, const Eigen::MatrixXd& i, const Eigen::VectorXd& min, const Eigen::VectorXd& max) : Controller<T>(p, d, i, min, max)
    {
        
    }

    template class PID<std::chrono::hours>;
    template class PID<std::chrono::minutes>;
    template class PID<std::chrono::seconds>;
    template class PID<std::chrono::milliseconds>;
    template class PID<std::chrono::microseconds>;
    template class PID<std::chrono::nanoseconds>;
}