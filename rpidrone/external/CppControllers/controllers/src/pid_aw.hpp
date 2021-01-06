#include "controller.hpp"


#ifndef CONTROLLERS_PID_AW_H
#define CONTROLLERS_PID_AW_H

namespace controllers {

    template <typename T = std::chrono::milliseconds>
    class PID_AW : public Controller<T>
    {
    private:
        const Eigen::VectorXd p_aw_;
    protected:
        /*
        Method to calculate the control value of the system based on the desired and current value
            
        @param err: The current system error
        @out: The vector with control values to reach the new desired state
        */
        void control(const Eigen::VectorXd& err, Eigen::VectorXd& out);

    public:
        /*
        Constructor
            
        @param p: The P value of the controller
        @param d: The D value of the controller
        @param i: The I value of the controller
        @param p_aw P value of anti windup
        @param min: Min value of the controller output
        @param max: Max value of the controller output
        */
        PID_AW(const Eigen::MatrixXd& p, const Eigen::MatrixXd& d, const Eigen::MatrixXd& i, const Eigen::VectorXd& p_aw,const Eigen::VectorXd& min, const Eigen::VectorXd& max);
    };
}
#endif
