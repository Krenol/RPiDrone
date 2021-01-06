#include "pi.hpp"

namespace controllers
{
    template<typename T>
    void PI<T>::control(const Eigen::VectorXd& err, Eigen::VectorXd& out) 
    {
        Eigen::VectorXd data;
        Controller<T>::calculateP(err, out);
        Controller<T>::calculateI(err, data);
        out = out + data;
    }
    
    template<typename T>
    PI<T>::PI(const Eigen::MatrixXd& p, const Eigen::MatrixXd& i) : Controller<T>(p, Eigen::MatrixXd::Zero(1,1), i)
    {
        
    }
    
    template<typename T>
    PI<T>::PI(const Eigen::MatrixXd& p, const Eigen::MatrixXd& i, const Eigen::VectorXd& min, const Eigen::VectorXd& max) : Controller<T>(p, Eigen::MatrixXd::Zero(1,1), i, min, max)
    {
        
    }

    template class PI<std::chrono::hours>;
    template class PI<std::chrono::minutes>;
    template class PI<std::chrono::seconds>;
    template class PI<std::chrono::milliseconds>;
    template class PI<std::chrono::microseconds>;
    template class PI<std::chrono::nanoseconds>;
}