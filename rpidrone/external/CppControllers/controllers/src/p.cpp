#include "p.hpp"

namespace controllers
{   
    template<typename T>
    void P<T>::control(const Eigen::VectorXd& err, Eigen::VectorXd& out) 
    {
        Controller<T>::calculateP(err, out);
    }

    template<typename T>
    P<T>::P(const Eigen::MatrixXd& p) : Controller<T>(p, Eigen::VectorXd::Zero(1,1), Eigen::VectorXd::Zero(1,1))
    {
        
    }
    
    template<typename T>
    P<T>::P(const Eigen::MatrixXd& p, const Eigen::VectorXd& min, const Eigen::VectorXd& max) : Controller<T>(p, Eigen::MatrixXd::Zero(1,1), Eigen::MatrixXd::Zero(1,1), min, max)
    {
        
    }

    template class P<std::chrono::hours>;
    template class P<std::chrono::minutes>;
    template class P<std::chrono::seconds>;
    template class P<std::chrono::milliseconds>;
    template class P<std::chrono::microseconds>;
    template class P<std::chrono::nanoseconds>;
}