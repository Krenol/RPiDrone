#include <mutex>
#include <chrono>
#include <atomic>
#include <Eigen/Dense>

#ifndef CONTROLLERS_CONTROLLER_H
#define CONTROLLERS_CONTROLLER_H

namespace controllers {

    //https://stackoverflow.com/a/28876046
    template <typename T>
    struct is_chrono_duration
    {
        static constexpr bool value = false;
    };

    template <typename Rep, typename Period>
    struct is_chrono_duration<std::chrono::duration<Rep, Period>>
    {
        static constexpr bool value = true;
    };

    template <typename T = std::chrono::milliseconds>
    class Controller
    {
        static_assert(is_chrono_duration<T>::value, "T not derived from std::chrono::duration");
    private:
        const Eigen::MatrixXd kp_, kd_, ki_;
        const bool bounded_;
        std::atomic_bool first_call_;
        std::atomic<double> dt_{0};
        std::chrono::steady_clock::time_point last_call_;
        std::mutex mtx_;
        /*
        Method to set dt_ for the integral / derivate term
        */
        void setDt();

        /*
        Private constructor
            
        @param p: The P value of the controller
        @param d: The D value of the controller
        @param i: The I value of the controller
        @param min: Min value of the controller output
        @param max: Max value of the controller output
        @param bounded: True if min/max is set else false
        */
        Controller(const Eigen::MatrixXd& p, const Eigen::MatrixXd& d, const Eigen::MatrixXd& i, const Eigen::VectorXd& min, const Eigen::VectorXd& max, bool bounded);

    protected: 
        Eigen::VectorXd integral_, last_err_, min_, max_;
        /*
        Method that returns a bounded value accoridng to min/max
        Only if controller was initialized with min/max!
        @param value: The value to be bounded
        @param out Vector bounded values are written to
        */
        void bound(Eigen::VectorXd& values);

        /* 
        Method to calculate the P value of the controller

        @param err: The error of the system
        @out: The vector where the P output is to be written to
        */
        void calculateP(const Eigen::VectorXd& err, Eigen::VectorXd& out);

        /* 
        Method to calculate the D value of the controller

        @param err: The error of the system
        @out: The vector where the D output is to be written to
        */
        void calculateD(const Eigen::VectorXd& err, Eigen::VectorXd& out);

        /* 
        Method to calculate the I value of the controller

        @param err: The error of the system
        @out: The vector where the I output is to be written to
        */
        void calculateI(const Eigen::VectorXd& err, Eigen::VectorXd& out);

        /*
        Constructor
            
        @param p: The P value of the controller
        @param d: The D value of the controller
        @param i: The I value of the controller
        */
        Controller(const Eigen::MatrixXd& p, const Eigen::MatrixXd& d, const Eigen::MatrixXd& i);

        /*
        Constructor
            
        @param p: The P value of the controller
        @param d: The D value of the controller
        @param i: The I value of the controller
        @param min: Min value of the controller output
        @param max: Max value of the controller output
        */
        Controller(const Eigen::MatrixXd& p, const Eigen::MatrixXd& d, const Eigen::MatrixXd& i, const Eigen::VectorXd& min, const Eigen::VectorXd& max);

        /*
        Method to calculate the control value of the system based on the desired and current value
            
        @param err: The current system error
        @out: The vector with control values to reach the new desired state
        */
        virtual void control(const Eigen::VectorXd& err, Eigen::VectorXd& out) = 0;

    public:
        /*
        Method to calculate the control value of the system based on the desired and current value
            
        @param x_cur: The current value of the system
        @param x_desired: The desired value of the system
        @out The new control values of the system to reach it's desired state
        */
        void calculate(const Eigen::VectorXd& x_cur, const Eigen::VectorXd& x_desired, Eigen::VectorXd& out);
        
    };
}
#endif
