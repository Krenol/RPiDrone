#ifndef _PID_H_
#define _PID_H_

#include <math.h>

class PID {
    private:
        const int freqCut_{20};
        const float ki_, kd_, kp_, kaw_, min_, max_, rc_ = 1 / (2 * M_PI * freqCut_);
        float integral_{0}, last_error_{0}, last_d_{0};
        const bool is_bound_{false};
        bool first_call_ = true;
        long last_call_ = 0;

        long getDt();

    public:
        /**
         * Constructor for PID
         * @param kp Value for proportional part
         * @param kd Value for differential part
         * @param ki Value for integral part
         * @param kaw Anti windup param for controller
         */
        PID(float kp = 1.0, float kd = 1.0, float ki = 1.0) ;
        /**
         * Constructor for PID
         * @param kp Value for proportional part
         * @param kd Value for differential part
         * @param ki Value for integral part
         * @param kaw Anti windup param for controller
         * @param min Min value of the controller
         * @param max max value of the controller
         */
        PID(float kp, float kd, float ki, float kaw, float min, float max);

        /**
         * Method to get control value 
         * @param is The is state of the system
         * @param desired The desired state
         */
        float control(float is, float desired);


};

#endif /* _PID_H_ */

