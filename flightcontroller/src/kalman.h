#ifndef _KALMAN_H_
#define _KALMAN_H_

// see https://thepoorengineer.com/en/angle-control-absolute/

class Kalman {
    private:
        bool initialized_ = false;
        double m_xHatBar[2];
        double m_pBar[2][2];
        double m_k[2];
        double m_xHat[2];
        double m_p[2][2];
        double m_q[2][2];
        double m_r;
        long dt_, millis_;

        void setDt();
        void updateStep(double angularVel, double angle);

    public:
        void init(double xHat[2], double p[2][2], double q[2][2], double r);

        void zeroTime();

        double getAngles(double angularVel, double angle);
 
};

#endif /* _KALMAN_H_ */

