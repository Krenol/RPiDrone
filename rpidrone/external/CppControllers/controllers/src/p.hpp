#include "controller.hpp"


#ifndef CONTROLLERS_P_H
#define CONTROLLERS_P_H

namespace controllers {

    template <typename T = std::chrono::milliseconds>
    class P : public Controller<T>
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
            
        @param p: The P values of the controller
        */
        P(const Eigen::MatrixXd& p);

        /*
        Constructor
            
        @param p: The P values of the controller
        @param min: Min values of the controller output
        @param max: Max values of the controller output
        */
        P(const Eigen::MatrixXd& p, const Eigen::VectorXd& min, const Eigen::VectorXd& max);
    };
}
#endif
