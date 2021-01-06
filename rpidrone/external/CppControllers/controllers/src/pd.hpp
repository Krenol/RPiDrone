#include "controller.hpp"


#ifndef CONTROLLERS_PD_H
#define CONTROLLERS_PD_H

namespace controllers {

    template <typename T = std::chrono::milliseconds>
    class PD : public Controller<T>
    {

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
        */
        PD(const Eigen::MatrixXd& p, const Eigen::MatrixXd& d);

        /*
        Constructor
            
        @param p: The P value of the controller
        @param d: The D value of the controller
        @param min: Min value of the controller output
        @param max: Max value of the controller output
        */
        PD(const Eigen::MatrixXd& p, const Eigen::MatrixXd& d, const Eigen::VectorXd& min, const Eigen::VectorXd& max);
    };
}
#endif
