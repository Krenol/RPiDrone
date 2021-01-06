#include "pd.hpp"

namespace controllers
{
    template<typename T>
    void PD<T>::control(const Eigen::VectorXd& err, Eigen::VectorXd& out) 
    {
        Eigen::VectorXd data;
        Controller<T>::calculateP(err, out);
        Controller<T>::calculateD(err, data);
        out = out + data;
    }
    
    template<typename T>
    PD<T>::PD(const Eigen::MatrixXd& p, const Eigen::MatrixXd& d) : Controller<T>(p, d, Eigen::MatrixXd::Zero(1,1))
    {
        
    }
    
    template<typename T>
    PD<T>::PD(const Eigen::MatrixXd& p, const Eigen::MatrixXd& d, const Eigen::VectorXd& min, const Eigen::VectorXd& max) : Controller<T>(p, d, Eigen::MatrixXd::Zero(1,1), min, max)
    {
        
    }

    template class PD<std::chrono::hours>;
    template class PD<std::chrono::minutes>;
    template class PD<std::chrono::seconds>;
    template class PD<std::chrono::milliseconds>;
    template class PD<std::chrono::microseconds>;
    template class PD<std::chrono::nanoseconds>;
}