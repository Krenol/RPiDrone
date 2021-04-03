#ifndef _PID_H_
#define _PID_H_


class PID {
    private:
        int freqCut_;
        float ki_, kd_, kp_, kaw_, min_, max_, rc_, integral_, last_error_, last_d_;
        bool first_call_, is_bound_, init_;
        long last_call_;

        long getDt();

        void init();

    public:
        PID();

        /**
         * Method to get control value 
         * @param is The is state of the system
         * @param desired The desired state
         */
        float control(float is, float desired);

        void set(float kp, float kd, float ki, float kaw, float min, float max);

        void set(float kp, float kd, float ki);


};

#endif /* _PID_H_ */

