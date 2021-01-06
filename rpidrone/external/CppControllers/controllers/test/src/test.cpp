#include "controllers/controllers.hpp"
#include <iostream> 
#include <unistd.h>

int main() {
    Eigen::MatrixXd p(2,1), d(2,1), i(2,1);
    Eigen::VectorXd aw(1), min(1), max(1), p_aw(1);
    p(0,0) = 2.123;
    p(1,0) = 1.12415;
    d(0,0) = 1.123;
    d(1,0) = 0.12415;
    i(0,0) = 0.123;
    i(1,0) = 0.012415;
    aw(0) = 0.2;
    min(0) = 1;
    max(0) = 100;
    p_aw(0) = 0.95;
    controllers::P<std::chrono::seconds> p_c(p);
    controllers::PD<std::chrono::seconds> pd_c(p, d);
    controllers::PI<std::chrono::seconds> pi_c(p, i);
    controllers::PID<std::chrono::seconds> pid_c(p, d, i, min, max);
    controllers::PID_AW<std::chrono::seconds> pidaw_c(p, d, i, p_aw, min, max);
    Eigen::VectorXd x_s(2), x_e(2), x_o;
    x_s(0) = 2;
    x_s(1) = 1;
    for(int i = 10; i > 0; i--) {
        x_e(0) = -22 * i;
        x_e(1) = 10 / i;
        p_c.calculate(x_s, x_e, x_o);
        std::cout << "P control val: " << x_o << std::endl;
        pd_c.calculate(x_s, x_e, x_o);
        std::cout << "PD control val: " << x_o << std::endl;
        pi_c.calculate(x_s, x_e, x_o);
        std::cout << "PI control val: " << x_o << std::endl;
        pid_c.calculate(x_s, x_e, x_o);
        std::cout << "PID control val: " << x_o << std::endl;
        pidaw_c.calculate(x_s, x_e, x_o);
        std::cout << "PID_AW control val: " << x_o << std::endl << std::endl;
        sleep(1);
    }
    return 0;
}