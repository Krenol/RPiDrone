#include "controller.hpp"


#ifndef CONTROLLERS_PI_H
#define CONTROLLERS_PI_H

namespace controllers {

    template <typename T = std::chrono::milliseconds>
    class PI : public Controller<T>
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
        @param i: The I value of the controller
        */
        PI(const Eigen::MatrixXd& p, const Eigen::MatrixXd& i);

        /*
        Constructor
            
        @param p: The P value of the controller
        @param i: The I value of the controller
        @param min: Min value of the controller output
        @param max: Max value of the controller output
        */
        PI(const Eigen::MatrixXd& p, const Eigen::MatrixXd& i, const Eigen::VectorXd& min, const Eigen::VectorXd& max);
    };
}
#endif
